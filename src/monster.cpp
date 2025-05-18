#include <sstream>
#include <algorithm>
#include <utility>

#include <monster.h>
#include <tables.h>
#include <treasure.h>
#include <dice.h>
#include <common_types.h>
#include <dictionary.h>

Adndtk::Monster::Monster(const Adndtk::Defs::monster& monsterId,
    std::string_view uniqueName, const std::optional<Defs::monster_variant_type>& monsterVariant)
    : _id{monsterId}, _name{}, _frequencyId{}, _intelligence{}, _cliamteTerrain{},
        _planes{}, _organisation{}, _turnedAs{}, _activityCycle{}, _diet{},
        _alignment{}, _element{}, _dragonType{}, _monsterVariant{monsterVariant}, _uniqueName{uniqueName}
{
    auto info = Tables::monster::select_one<Defs::monster>("id", _id);
    if (!info.has_value())
    {
        std::stringstream ss{};
        ss << "Monster ID " << _id << " does not exists";
        throw std::runtime_error(ss.str());
    }

    _name = info.value().display_name.value_or(info.value().name);
    _fullName = info.value().name;
    _instanceName = (!instanceName.empty()) ? instanceName : _name;
    _frequencyId = info.value().frequency_id;
    _intelligence = get_intelligence_score(info.value().intelligence_from, info.value().intelligence_to);

    auto rsClimateTerrain = Adndtk::Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(Query::select_monster_climate_terrain, _id);
    for (const auto& r : rsClimateTerrain)
    {
        auto climate = r.as<Defs::climate>("climate_id");
        auto terrain = r.as<Defs::terrain>("terrain_id");
        _cliamteTerrain.emplace(std::make_pair(climate, terrain));
    }
    
    _planes = set_multiple_feature<Defs::plane_of_existence>(_id, Query::select_monster_plane, "plane_id");
    _organisation = set_multiple_feature<Defs::organisation>(_id, Query::select_monster_organisation, "organisation_id");
    _activityCycle = set_multiple_feature<Defs::activity_cycle>(_id, Query::select_monster_activity_cycle, "activity_cycle_id");
    _diet = set_multiple_feature<Defs::diet>(_id, Query::select_monster_diet, "diet_id");
    
    _alignment = set_optional_feature<Defs::moral_alignment>(_id, Query::select_monster_alignment, "alignment_id");
    _element = set_optional_feature<Defs::element>(_id, Query::select_monster_elemntal, "element_id");
    _dragonType = set_optional_feature<Defs::dragon_type>(_id, Query::select_monster_dragon, "type");
    _turnedAs = set_optional_feature<Defs::turnable>(_id, Query::select_monster_undead, "turned_as");

    auto rsTreasure = Adndtk::Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(Query::select_monster_treasure, _id);
    for (const auto& r : rsTreasure)
    {
        auto treasureClass = r.try_as<Defs::treasure_class>("treasure_class_id");
        auto multiplier = r.try_as<double>("multiplier");
        auto diceNumber = r.try_as<short>("multiplier_dice_num");
        auto dieFaces = r.try_as<Defs::die>("multiplier_die_faces");
        auto treasureLocation = r.try_as<Defs::treasure_location>("location_id");
        auto treasureComponent = r.try_as<Defs::treasure_component>("treasure_component_id");
        // _cliamteTerrain.emplace(std::make_pair(climate, terrain));
    }

    auto rsAcVariants = Adndtk::Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(Query::select_monster_ac, _id);
    for (const auto& r : rsAcVariants)
    {
        auto acValue = r.try_as<AC>("ac");
        auto acVariant = r.try_as<std::string>("ac_variant");
        if (acValue)
        {
            _acVariants[acValue.value()] = acVariant.value_or("");
        }
    }

    QueryResultSet variantInfo;
    if (_monsterVariant.has_value())
    {
        auto varQueryId{Query::select_monster_variant_type};
        variantInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::monster, Defs::monster_variant_type>(varQueryId, _id, _monsterVariant.value());

        if (variantInfo.size() == 0)
        {
            std::stringstream ss{};
            ss << "Monster variant " << Dictionary::to_string(_monsterVariant.value()) << " is not valid";
            throw std::runtime_error(ss.str());
        }
    }
    else
    {
        auto varQueryId{Query::select_monster_variant_default};
        variantInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::monster>(varQueryId, _id);

        auto variantType = variantInfo[0].try_as<Defs::monster_variant_type>("monster_variant");
        if (variantType)
        {
            _monsterVariant = variantType.value();
        }
    }

    auto hd = variantInfo[0].try_as<Adndtk::HP>("hd");
    auto die_faces = variantInfo[0].try_as<Defs::die>("die_faces");
    auto die_modifier = variantInfo[0].try_as<short>("die_modifier");
    auto hp = variantInfo[0].try_as<Adndtk::HP>("hp");
    auto hp_to = variantInfo[0].try_as<Adndtk::HP>("hp_to");

    _hpValueMax = get_hp_score(hd, die_faces, die_modifier, hp, hp_to);
    _hpValueCurrent = _hpValueMax;

    _thac0Value = variantInfo[0].try_as<THAC0>("thac0");
    _xpValue = variantInfo[0].try_as<THAC0>("xp");
}

Adndtk::Monster::Monster(const Adndtk::Defs::monster& monsterId,
    const std::optional<Defs::monster_variant_type>& monsterVariant)
    : Monster{monsterId, "", monsterVariant}
{
}

double Adndtk::Monster::get_treasure_multiplier(const std::optional<double>& multiplier, const std::optional<short>& diceNumber, const std::optional<Defs::die>& dieFaces)
{
    if (multiplier.has_value())
    {
        return multiplier.value();
    }
    if (diceNumber.has_value() && dieFaces.has_value())
    {
        return DiceSet::get_instance().roll(diceNumber.value(), dieFaces.value());
    }
    return 1;
}

std::optional<Adndtk::SkillValue> Adndtk::Monster::get_intelligence_score(const std::optional<short>& value_from, const std::optional<short>& value_to)
{
    if (!value_from.has_value())
    {
        return std::nullopt;
    }
    if (!value_to.has_value())
    {
        return SkillValue{Defs::skill::intelligence, value_from.value()};
    }
    
    auto score = Die::roll<short>(value_from.value(), value_to.value());
    return SkillValue(Defs::skill::intelligence, score);
}

Adndtk::HP Adndtk::Monster::get_hp_score(const std::optional<Adndtk::HP>& hd,
    const std::optional<Adndtk::Defs::die>& die_faces,
    const std::optional<short>& die_modifier,
    const std::optional<Adndtk::HP>& hp,
    const std::optional<Adndtk::HP>& hp_to)
{
    Adndtk::HP score{};
    if (hd.has_value())
    {
        Defs::die faces = die_faces.value_or(Defs::die::d8);
        score = Adndtk::DiceSet::get_instance().roll(hd.value(), faces);
        score += die_modifier.value_or(0);
    }
    else if (hp.has_value() && hp_to.has_value())
    {
        score = Adndtk::Die::roll(hp.value(), hp_to.value());
    }
    else if (hp.has_value())
    {
        score = hp.value();
    }
    return score;
}

std::optional<Adndtk::AC> Adndtk::Monster::ac() const
{
    auto minAC{Const::max_ac_value};
    for (const auto& [ac, variant] : _acVariants)
    {
        minAC = std::min(ac, minAC);
    }
    return minAC;
}

std::optional<Adndtk::AC> Adndtk::Monster::ac(std::string_view variant) const
{
    for (const auto& var : _acVariants)
    {
        if (var.second == variant)
        {
            return var.first;
        }
    }
    return std::nullopt;
}

std::optional<Adndtk::Defs::monster_variant_type> Adndtk::Monster::variant() const
{
    return _monsterVariant;
}

Adndtk::HP Adndtk::Monster::hp() const
{
    return _hpValueCurrent;
}

Adndtk::HP Adndtk::Monster::hp_max() const
{
    return _hpValueMax;
}

std::optional<Adndtk::THAC0> Adndtk::Monster::thac0() const
{
    return _thac0Value;
}

std::optional<Adndtk::XP> Adndtk::Monster::xp() const
{
    return _xpValue;
}