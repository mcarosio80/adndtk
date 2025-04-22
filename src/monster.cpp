#include <monster.h>
#include <tables.h>
#include <sstream>

Adndtk::Monster::Monster(const Adndtk::Defs::monster& monsterId)
    : _id{monsterId}
{
    auto info = Tables::monster::select_one<Defs::monster>("id", _id);
    if (!info.has_value())
    {
        std::stringstream ss{};
        ss << "Monster ID " << _id << " does not exists";
        throw std::runtime_error(ss.str());
    }

    _name = info.value().display_name.value_or(info.value().name);
    _frequencyId = info.value().frequency_id;
    _intelligence = get_intelligence_score(info.value().intelligence_from, info.value().intelligence_to);

    auto rsClimateTerrain = Adndtk::Cyclopedia::get_instance().exec_prepared_statement(Query::select_monster_climante_terrain, _id);
    for (const auto& r : rsClimateTerrain)
    {
        auto climate = r.as<Defs::climate>("climate_id");
        auto terrain = r.as<Defs::terrain>("terrain_id");
        _cliamteTerrain.emplace(std::make_pair(climate, terrain));
    }
    
    _planes = set_multiple_feature<Defs::plane_of_existence>(_id, Query::select_monster_plane, "plane_id");
    _organisation = set_multiple_feature<Defs::organisation>(_id, Query::select_monster_organisation, "organisation_id");
    _activityCycle = set_multiple_feature<Defs::activity_cycle>(_id, Query::select_monster_activity_cycle, "activity_cycle_id");
    
    _alignment = set_optional_feature<Defs::moral_alignment>(_id, Query::select_monster_alignment, "alignment_id");
    _element = set_optional_feature<Defs::element>(_id, Query::select_monster_elemntal, "element_id");
    _dragonType = set_optional_feature<Defs::monster_dragon_type>(_id, Query::select_monster_dragon, "type");
    _turnedAs = set_optional_feature<Defs::turnable>(_id, Query::select_monster_undead, "turned_as");

    auto rsTreasure = Adndtk::Cyclopedia::get_instance().exec_prepared_statement(Query::select_monster_treasuret, _id);
    for (const auto& r : rsTreasure)
    {
        auto treasureClass = r.as<Defs::treasure_class>("treasure_class_id");
        auto multiplier = r.as<double>("multiplier");
        auto diceNumber = r.as<short>("multiplier_dice_num");
        auto dieFaces = r.as<Defs::die>("multiplier_die_faces");
        auto treasureLocation = r.as<Defs::treasure_location>("location_id");
        auto treasureComponent = r.as<Defs::treasure_component>("treasure_component_id");
        // _cliamteTerrain.emplace(std::make_pair(climate, terrain));
    }
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