#include <monster.h>
#include <tables.h>
#include <treasure.h>
#include <dice.h>
#include <sstream>

Adndtk::Monster::Monster(const Adndtk::Defs::monster& monsterId)
    : _id{monsterId}, _name{}, _frequencyId{}, _intelligence{}, _cliamteTerrain{},
        _planes{}, _organisation{}, _turnedAs{}, _activityCycle{}, _diet{},
        _alignment{}, _element{}, _dragonType{}
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
    _dragonType = set_optional_feature<Defs::monster_dragon_type>(_id, Query::select_monster_dragon, "type");
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
}

double Adndtk::Monster::get_treasure_multiplier(const std::optional<double>& multiplier, const std::optional<short>& diceNumber, const std::optional<Defs::die>& dieFaces)
{
    if (multiplier.has_value())
    {
        return multiplier.value();
    }
    if (diceNumber.has_value() && dieFaces.has_value())
    {
        Die d{dieFaces.value()};
        return d.roll(diceNumber.value());
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