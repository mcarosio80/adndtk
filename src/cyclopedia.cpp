#include <cyclopedia.h>
#include <common_types.h>
#include <skills.h>
#include <skill_creator.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/allocators.h"

#include <sstream>
#include <cstring>

bool Adndtk::Cyclopedia::_initialised = false;


Adndtk::Cyclopedia& Adndtk::Cyclopedia::get_instance()
{
    static Cyclopedia _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

Adndtk::QueryResultSet Adndtk::Cyclopedia::from_json(const char *jsonText)
{
    rapidjson::Document jsonDoc;
    jsonDoc.Parse(jsonText);
    
    QueryResultSet set;
    for (rapidjson::Value::ConstValueIterator it = jsonDoc.Begin(); it != jsonDoc.End(); ++it)
    {
        auto record = it->GetObject();
        Adndtk::QueryResult result;
        for (auto& m : record)
        {
            auto key = m.name.GetString();
            result.add(key, m.value);
        }
        set += result;
    }
    return set;
}

bool Adndtk::Cyclopedia::init()
{
    int ret = sqlite3_open_v2(Settings::database_path, &_dbConn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
    
    bool ok = check_state(ret);
    if (ok)
    {
        prepare_statement("select 'id', id, 'acronym', acronym, 'description', description from coin where id = ?", Query::select_coin);

        prepare_statement("select 'min', min, 'max', max from skill_requisite_values where requisite_id = 0 and skill_id = ?", Query::select_skill_boundaries_default);
        prepare_statement("select 'min', min, 'max', max from skill_requisite_values where requisite_id = 1 and skill_id = ? and object = ?", Query::select_skill_boundaries_class_type);
        prepare_statement("select 'min', min, 'max', max from skill_requisite_values where requisite_id = 2 and skill_id = ? and object = ?", Query::select_skill_boundaries_class);
        prepare_statement("select 'min', min, 'max', max from skill_requisite_values where requisite_id = 3 and skill_id = ? and object = ?", Query::select_skill_boundaries_race);

        prepare_statement("select 'id', id, 'class_type_id', class_type_id, 'long_name', long_name, 'short_name', short_name, 'acronym', acronym from character_class", Query::select_character_classes);
        prepare_statement("select 'id', id, 'class_type_id', class_type_id, 'long_name', long_name, 'short_name', short_name, 'acronym', acronym from character_class where id = ?", Query::select_character_class);
        prepare_statement("select 'description', description, 'title_level', title_level, 'title', title, 'hit_dice', hit_dice, 'hp_after_title', hp_after_title from character_class_type where id = ?", Query::select_character_class_type);

        prepare_statement("select 'race_id', race_id, 'skill_id', skill_id, 'value', value from skill_modifier where race_id = ? and skill_id = ?", Query::select_skill_modifier);
        prepare_statement("select 'skill_value_required', skill_value_required from SCHOOL_OF_MAGIC where class_id = ?", Query::select_school_of_magic_skill_requisite);

        prepare_statement("select 'class_id', class_id, 'level', level, 'score', score from LEVEL_ADVANCEMENT order by class_id, level", Query::select_level_advancement);
        prepare_statement("select 'class_id', class_id, 'score', score from LEVEL_ADVANCEMENT_FACTOR", Query::select_level_advancement_factor);

        prepare_statement("select 'class_id', class_id, 'level_limit', level_limit from CLASS_AVAILABILITY where LEVEL_LIMIT is not null and CLASS_ID & ? and race_id = ?", Query::select_class_limits);

        prepare_statement("select 'class_type_id', sts.class_type_id, 'level', sts.level, 'saving_throw_id', st.id, 'score', sts.score from SAVING_THROW_SCORES sts inner join SAVING_THROW st on st.score = sts.SAVING_THROW_ID", Query::select_saving_throws);

        prepare_statement("select 'score', score, 'factor', factor from THACO t where CLASS_TYPE_ID = ?", Query::select_thaco);

        prepare_statement("select 'coin_from', coin_from, 'coin_to', coin_to, 'exchange_value', exchange_value from coin_exchange_values", Query::select_coin_exchange_rates);

        prepare_statement("select 'name', name, 'type', type, 'cost_coin', cost_coin, 'cost_min', cost_min, 'cost_max', cost_max, 'weight', weight, 'body_slot', body_slot, 'capacity_limit', capacity_limit from equipment e left join BODY_SLOT b on b.id = e.body_slot where e.id = ?", Query::select_equipment);

        prepare_statement("select 'id', id, 'ability_score_from', ability_score_from, 'excp_from', excp_from, 'ability_score_to', ability_score_to, 'excp_to', excp_to, 'hit_modifier', hit_modifier, 'damage_adjustment', damage_adjustment, 'weight_allowance', weight_allowance, 'maximum_press', maximum_press, 'open_doors', open_doors, 'open_doors_special', open_doors_special, 'bend_bars_lift_gates', bend_bars_lift_gates, 'note', note from strength_stats", Query::select_all_strength_stats);
        prepare_statement("select 'ability_score', ability_score, 'reaction_adjustment', reaction_adjustment, 'missile_attack_adjustment', missile_attack_adjustment, 'defensive_adjustment', defensive_adjustment from dexterity_stats", Query::select_all_dexterity_stats);
        prepare_statement("select 'ability_score', ability_score, 'hit_point_adjustment', hit_point_adjustment, 'hit_point_adjustment_warriors', hit_point_adjustment_warriors, 'rolls_upgrade', rolls_upgrade, 'system_shock', system_shock, 'resurrection_survival', resurrection_survival, 'poison_save', poison_save, 'regeneration_points', regeneration_points, 'regeneration_turns', regeneration_turns from constitution_stats", Query::select_all_constitution_stats);
        prepare_statement("select 'ability_score', ability_score, 'number_of_languages', number_of_languages, 'spell_level', spell_level, 'chance_to_learn_spell', chance_to_learn_spell, 'max_number_of_spells_per_level', max_number_of_spells_per_level, 'illusion_immunity', illusion_immunity from intelligence_stats", Query::select_all_intelligence_stats);
        prepare_statement("select 'ability_score', ability_score, 'magical_defence_adjustment', magical_defence_adjustment, 'bonus_spell_level_1', bonus_spell_level_1, 'bonus_spell_level_2', bonus_spell_level_2, 'bonus_spell_level_3', bonus_spell_level_3, 'bonus_spell_level_4', bonus_spell_level_4, 'bonus_spell_level_5', bonus_spell_level_5, 'bonus_spell_level_6', bonus_spell_level_6, 'bonus_spell_level_7', bonus_spell_level_7, 'chance_of_spell_failure', chance_of_spell_failure, 'spell_immunity', spell_immunity from wisdom_stats", Query::select_all_wisdom_stats);
        prepare_statement("select 'ability_score', ability_score, 'maximum_number_of_henchmen', maximum_number_of_henchmen, 'loyalty_base', loyalty_base, 'reaction_adjustment', reaction_adjustment from charisma_stats", Query::select_all_charisma_stats);
        prepare_statement("select 'id', id, 'name', name, 'acronym', acronym, 'base_movement_rate', base_movement_rate from race", Query::select_all_race);
        prepare_statement("select 'id', id, 'class_type_id', class_type_id, 'long_name', long_name, 'short_name', short_name, 'acronym', acronym from character_class", Query::select_all_character_class);
        prepare_statement("select 'id', id, 'name', name from skill", Query::select_all_skill);
        prepare_statement("select 'id', id, 'name', name, 'rank', rank, 'moral_alignment', moral_alignment, 'realm', realm, 'plane', plane, 'sex', sex, 'activity', activity, 'followers_race', followers_race, 'cult', cult from deity", Query::select_all_deity);
        prepare_statement("select 'id', id, 'turnable_code', turnable_code, 'level', level, 'effect', effect, 'value', value from TURN_UNDEAD", Query::select_all_turn_undead);
        prepare_statement("select 'id', id, 'name', name, 'type', type, 'cost_coin', cost_coin, 'cost_min', cost_min, 'cost_max', cost_max, 'weight', weight, 'body_slot', body_slot from equipment", Query::select_all_equipment);
        prepare_statement("select 'id', id, 'description', description, 'title_level', title_level, 'title', title, 'hit_dice', hit_dice, 'hp_after_title', hp_after_title from character_class_type", Query::select_all_character_class_type);
        prepare_statement("select 'id', id, 'acronym', acronym, 'description', description from coin", Query::select_all_coin);
        prepare_statement("select 'id', id, 'name', name, 'acronym', acronym from moral_alignment", Query::select_all_moral_alignment);

        prepare_statement("select 'id', id, 'base_score', base_score from thief_ability", Query::select_thief_ability_base_scores);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_armour_adjustments where armour_id = ?", Query::select_thieving_skill_armour_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_armour_adjustments where armour_id is null", Query::select_thieving_skill_no_armour_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_dexterity_adjustments where dexterity = ?", Query::select_thieving_skill_dexterity_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_racial_adjustments where race_id = ?", Query::select_thieving_skill_racial_adjustments);

        prepare_statement("select 'name', name, 'level', level from wizard_spell where id = ?", Query::select_wizard_spell);
        prepare_statement("select 'school_id', school_id from wizard_spell_school where spell_id = ?", Query::select_wizard_spell_school);
        prepare_statement("select 'school_id', school_id from school_of_magic_access where class_id = ? and school_id = ?", Query::select_school_of_magic_access);
        prepare_statement("select 'race_id', a.race_id from CHARACTER_CLASS c "
                            "inner join CLASS_AVAILABILITY a on a.class_id = c.id "
                            "where c.id = ? and a.race_id = ?", Query::select_school_of_magic_per_race);
        prepare_statement("select 'level', level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4, 'spell_level_5', spell_level_5, 'spell_level_6', spell_level_6, 'spell_level_7', spell_level_7, 'spell_level_8', spell_level_8, 'spell_level_9', spell_level_9 from wizard_spell_progression where level = ?", Query::select_wizard_spell_progression);

        prepare_statement("select 'name', name, 'level', level from priest_spell where id = ?", Query::select_priest_spell);
        prepare_statement("select 'sphere_id', sphere_id from priest_spell_sphere where spell_id = ?", Query::select_priest_spell_sphere);
        prepare_statement("select 'sphere_id', sphere_id , 'access_level', access_level , 'access_mode', access_mode from sphere_access_per_class where class_id = ? and sphere_id = ?", Query::select_sphere_access_per_class);
        prepare_statement("select 'level', level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4, 'spell_level_5', spell_level_5, 'spell_level_6', spell_level_6, 'spell_level_7', spell_level_7 from priest_spell_progression where level = ?", Query::select_priest_spell_progression);

        prepare_statement("select distinct 'id', ps.id, 'access_level', c.access_level, 'access_mode', c.access_mode from sphere_access_per_class c "
                            "inner join priest_spell_sphere s on s.sphere_id = c.sphere_id "
                            "inner join priest_spell ps on ps.id = s.spell_id "
                            "where c.class_id = ? and ps.level = ?", Query::select_priest_spells_per_class_level);
        
        prepare_statement("select distinct 'id', ps.id from DEITY_SPHERE_OF_INFLUENCE dsi "
                            "inner join PRIEST_SPELL_SPHERE pss on pss.SPHERE_ID = dsi.SPHERE_ID "
                            "inner join PRIEST_SPELL ps on ps.ID = pss.SPELL_ID and ps.LEVEL = ? "
                            "WHERE dsi.deity_id = ?", Query::select_priest_spells_per_level_deity);

        prepare_statement("select 'level', level, 'casting_level', casting_level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4 from paladin_spell_progression where level = ?", Query::select_paladin_spell_progression);
        prepare_statement("select 'level', level, 'casting_level', casting_level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3 from ranger_abilities where level = ?", Query::select_ranger_spell_progression);            
        
        prepare_statement("select 'id', id, 'base_movement_rate', base_movement_rate from race", Query::select_race_base_movement);            
        prepare_statement("select 'strength_to', strength_to, 'exc_strength_to', exc_strength_to, 'load', load, 'movement_rate_1', movement_rate_1, 'movement_rate_2', movement_rate_2 from modified_movement_rates", Query::select_modified_movement_rate);            
        
        prepare_statement("select 'die_number', die_number, 'die_faces', die_faces, 'die_base', die_base, 'multiplier', multiplier from starting_money where class_type_id = ?", Query::select_starting_money);

        prepare_statement("select 'skill_id', skill_id from primary_skill where class_id = ?", Query::select_primary_skills);

        prepare_statement("select 'id', id from wizard_spell where level = ?", Query::select_wizard_spells_by_level);
        
        prepare_statement("select 'level', level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4, 'spell_level_5', spell_level_5, 'spell_level_6', spell_level_6 from bard_spell_progression where level = ?", Query::select_bard_spell_progression);
        
        prepare_statement("select 'moral_alignment', moral_alignment, 'deity_id', deity_id from worshipper_alignment where moral_alignment = ?", Query::select_deities_by_moral_alignment);

        prepare_statement("select 'class_id', class_id, 'alignment_id', alignment_id from moral_alignment_per_class where class_id = ?", Query::select_moral_alignments_by_class);
        
        prepare_statement("select 'moral_alignment', moral_alignment from worshipper_alignment where deity_id = ?", Query::select_worshipper_alignments);
        
        prepare_statement("select 'height_base_male', height_base_male, 'height_base_female', height_base_female, 'height_dice_number', height_dice_number, 'height_dice_faces', height_dice_faces, 'height_dice_number_2', height_dice_number_2, 'height_dice_faces_2', height_dice_faces_2, 'weight_base_male', weight_base_male, 'weight_base_female', weight_base_female, 'weight_dice_number', weight_dice_number, 'weight_dice_faces', weight_dice_faces, 'weight_dice_number_2', weight_dice_number_2, 'weight_dice_faces_2', weight_dice_faces_2, 'age_starting', age_starting, 'age_dice_number', age_dice_number, 'age_dice_faces', age_dice_faces, 'age_maximum', age_maximum, 'age_maximum_dice_number', age_maximum_dice_number, 'age_maximum_dice_faces', age_maximum_dice_faces, 'middle_age', middle_age, 'old_age', old_age, 'venerable_age', venerable_age from racial_stats where race_id = ?", Query::select_racial_stats);
        
        prepare_statement("select 'id', id, 'description', description, 'strength_modifier', strength_modifier, 'dexterity_modifier', dexterity_modifier, 'constitution_modifier', constitution_modifier, 'intelligence_modifier', intelligence_modifier, 'wisdom_modifier', wisdom_modifier, 'charisma_modifier', charisma_modifier from aging_effects", Query::select_aging_effects);
        
        prepare_statement("select 'id', id from calendar", Query::select_calendars);
        prepare_statement("select 'name', name, 'campaign_settings_id', campaign_settings_id from calendar where id = ?", Query::select_calendar);
        prepare_statement("select 'id', id, 'name', name, 'acronym', acronym, 'calendar_id', calendar_id, 'base_year', base_year from calendar_reckoning where calendar_id = ?", Query::select_calendar_reckonings);
        prepare_statement("select 'id', id, 'name', name, 'duration', duration from calendar_month where calendar_id = ?", Query::select_calendar_months);
        prepare_statement("select 'id', id, 'name', name, 'year_day', year_day, 'leap_year_day', leap_year_day from calendar_special_day where calendar_id = ?", Query::select_calendar_special_days);
        prepare_statement("select 'name', name from roll_of_the_year where year = ?", Query::select_roll_of_the_year);

        prepare_statement("select 'id', id from gem where type = ?", Query::select_gems_by_type);
        prepare_statement("select 'id', g.id, 'name', g.name, 'type', g.type, 'value_range_coin', g.value_range_coin, 'value_range_from', g.value_range_from, 'value_range_to', g.value_range_to, 'type_name', gt.name, 'base_coin', gt.base_coin, 'base_value', gt.base_value, 'probability_from', gt.probability_from, 'probability_to', gt.probability_to from gem g inner join gem_type gt on gt.id = g.type where g.id = ?", Query::select_gem);
        prepare_statement("select 'id', id, 'name', name, 'base_coin', base_coin, 'base_value', base_value, 'probability_from', probability_from, 'probability_to', probability_to from gem_type where ? between probability_from and probability_to", Query::select_gem_type);
        prepare_statement("select 'id', id, 'value_coin', value_coin, 'value_from', value_from, 'value_to', value_to from object_of_art where ? between probability_from and probability_to", Query::select_object_of_art_by_probability);
        prepare_statement("select 'id', i.id, 'name', i.name, 'nature', i.nature, 'type', i.type, 'xp_value', i.xp_value, 'item_type', i.item_type, 'equipment_id', i.equipment_id, 'unidentified_name', un.name from magical_item i inner join magical_item_unidentified_name un on un.id = i.unidentified_id where i.id = ?", Query::select_magical_item);
        prepare_statement("select 'allowed_class_id', allowed_class_id, 'allowed_class_type_id', allowed_class_type_id, 'allowed_alignment_id', allowed_alignment_id from magical_item_limitation where item_id = ?", Query::select_magical_item_limitations);
        prepare_statement("select 'id', id, 'name', name from magical_item_type where ? between probability_from and probability_to", Query::select_magical_item_type_by_probability);
        prepare_statement("select 'id', i.id, 'found_probability_from', t.probability_from, 'found_probability_to', t.probability_to from MAGICAL_ITEM i inner join MAGICAL_ITEM_TYPE t on t.id = i.type inner join MAGICAL_ITEM_PROBABILITY p on p.MAGICAL_ITEM_ID = i.id where i.type = ? and ? between p.subtable_from and p.subtable_to and ? between p.probability_from and p.probability_to", Query::select_magical_item_by_type_and_probability);
        prepare_statement("select 'id', id from magical_item where type = ?", Query::select_magical_items_by_type);
        prepare_statement("select 'id', id from treasure_component", Query::select_treasure_components);
        prepare_statement("select 'count_from', count_from, 'count_to', count_to, 'probability', probability, 'nature', nature, 'additional_nature', additional_nature, 'additional_count', additional_count from treasure_composition where treasure_class = ? and component = ?", Query::select_treasure_composition);

        prepare_statement("select 'class_id', class_id from class_availability where race_id = ?", Query::select_class_availability_per_race);

        // prepare_statement("select r.id as id, "
	    //     "case when rr.min > rd.min then rr.min else rd.min end as mirt_skill, "
	    //     "case when rr.max < rd.max then rr.max else rd.max end as max_skill "
        //     "from race r left join SKILL s "
        //     "inner join SKILL_REQUISITE_VALUES rd on rd.REQUISITE_ID = 0 and rd.SKILL_ID = s.id "
        //     "left join SKILL_REQUISITE_VALUES rr on rr.REQUISITE_ID = 3 and rr.SKILL_ID = s.id and rr.object = r.id "
        //     "where r.id = ?", Query::select_skill_boundaries_per_race);

        load_advancement_table();
    }
    return ok;
}

Adndtk::Cyclopedia::Cyclopedia()
    : _dbConn(nullptr)
{
}

Adndtk::Cyclopedia::~Cyclopedia()
{
    int ret = sqlite3_close_v2(_dbConn);
    check_state(ret);
}

bool Adndtk::Cyclopedia::is_multiclass(const Defs::character_class& cls)
{
    return split<Defs::character_class>(cls).size() > 1;
}

bool Adndtk::Cyclopedia::check_state(int return_code)
{
    if (return_code != SQLITE_OK)
	{
        std::stringstream ss;
        ss << sqlite3_errstr(return_code) << ": " << sqlite3_errmsg(_dbConn);
        sqlite3_close_v2(_dbConn);
        ErrorManager::get_instance().error(ss.str().c_str());
        return false;
    }
    return true;
}

void Adndtk::Cyclopedia::prepare_statement(const char *stmt, const Query& queryId, const QueryType& queryType/* = Settings::query_type*/)
{
    sqlite3_stmt *pStmt = nullptr;
    std::string selectStmt{stmt, std::strlen(stmt)};

    if (queryType == QueryType::json)
    {
        std::string token = "select ";
        auto it = selectStmt.find(token);
        selectStmt.replace(it, token.length(), "select json_group_array(json_object(");
        
        token = " from";
        it = selectStmt.find(token);
        selectStmt.replace(it, token.length(), ")) from");
    }

    int ret = sqlite3_prepare_v2(_dbConn, selectStmt.c_str(), static_cast<int>(selectStmt.length()), &pStmt, NULL);

    if (check_state(ret))
    {
        _statements[queryId] = pStmt;
    }
}

Adndtk::QueryResultSet Adndtk::Cyclopedia::parse_json_result(sqlite3_stmt* stmt)
{
    QueryResultSet resultSet;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto colValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        resultSet = from_json(colValue);
    }
    return resultSet;
}

Adndtk::QueryResultSet Adndtk::Cyclopedia::parse_tabular_result(sqlite3_stmt* stmt)
{
    int rowCount = 0;
    int columnCount = sqlite3_column_count(stmt);

    std::vector<std::string> captions;
    for (int col=0; col<columnCount; ++col)
    {
        std::string colName{sqlite3_column_name(stmt, col)};
        captions.push_back(colName);
    }

    QueryResultSet resultSet;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        QueryResult res;
        for (int col = 0; col<columnCount; ++col)
        {
            std::string colValue{reinterpret_cast<const char*>(sqlite3_column_text(stmt, col))};
            res.add(captions[col], colValue);
        }
        resultSet += res;
    }
    return resultSet;
}

int Adndtk::Cyclopedia::onSqliteDataRetrieved(void *data, int argc, char **argv, char **columnName)
{
    Adndtk::QueryResultSet *set = reinterpret_cast<Adndtk::QueryResultSet*>(data);

    if (Adndtk::Settings::query_type == Adndtk::QueryType::json)
    {
        for (int i=0; i<argc; ++i)
        {
            auto v = from_json(argv[i]);
            set += v;
        }
    }
    else if (Settings::query_type == QueryType::plain)
    {
        QueryResult res;
        for (int i=0; i<argc; ++i)
        {
            res.add(columnName[i], argv[i]);
        }
        (*set) += res;
    }
    
    return 0;
}

Adndtk::QueryResultSet Adndtk::Cyclopedia::exec(const char* stmt)
{
    Adndtk::QueryResultSet set;
    char *error = nullptr;
    int rc = SQLITE_OK;
    
    while (rc == SQLITE_OK)
    {
        rc = sqlite3_exec(_dbConn, stmt, Cyclopedia::onSqliteDataRetrieved, (void*)&set, &error);
    }

    if (rc != SQLITE_DONE)
    {
        std::string msg{error};
        sqlite3_free(error);
        ErrorManager::get_instance().error(msg.c_str());
    }
    return set;
}

void Adndtk::Cyclopedia::load_advancement_table()
{
    auto advTable = exec_prepared_statement<>(Query::select_level_advancement);
    for (auto& r : advTable)
    {
        auto cls = static_cast<Defs::character_class>(r.as<int>("class_id"));
        auto lvl = static_cast<ExperienceLevel>(r.as<int>("level"));
        auto score = static_cast<XP>(r.as<int>("score"));

        _advTable.add_level(cls, lvl, score);
    }

    auto advFactors = exec_prepared_statement<>(Query::select_level_advancement_factor);
    for (auto& r : advFactors)
    {
        auto cls = static_cast<Defs::character_class>(r.as<int>("class_id"));
        auto score = static_cast<XP>(r.as<int>("score"));

        _advTable.set_advancement_factor(cls, score);
    }
}

Adndtk::Defs::character_class_type Adndtk::Cyclopedia::get_class_type(const Defs::character_class& cls)
{
    int clsId = static_cast<int>(cls);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Adndtk::Query::select_character_class, clsId);
    auto& clsInfo = rs[0];
    return static_cast<Defs::character_class_type>(clsInfo.as<int>("class_type_id"));
}

std::vector<Adndtk::Defs::character_class_type> Adndtk::Cyclopedia::get_class_types(const Defs::character_class& cls)
{
    auto types = get_class_type(cls);
    return split<Defs::character_class_type>(types);
}

std::set<Adndtk::Defs::moral_alignment> Adndtk::Cyclopedia::available_moral_alignments_by_mythos(const Defs::character_class& cls, const std::optional<Defs::deity>& deityId/*=std::nullopt*/) const
{
    if (cls != Defs::character_class::preist_of_specific_mythos || !deityId.has_value())
    {
        throw std::runtime_error("Unable to determine the available moral alignments");
    }

    auto deity = static_cast<int>(deityId.value());
    auto rsWorshipperAligns = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_worshipper_alignments, deity);
    
    std::set<Adndtk::Defs::moral_alignment> psmAligns;
    for (auto& a : rsWorshipperAligns)
    {
        auto alignId = static_cast<Defs::moral_alignment>(a.as<int>("moral_alignment"));
        psmAligns.emplace(alignId);
    }

    return psmAligns;
}

std::set<Adndtk::Defs::moral_alignment> Adndtk::Cyclopedia::available_moral_alignments_by_single_class(const Defs::character_class& cls) const
{
    if (cls == Defs::character_class::preist_of_specific_mythos)
    {
        throw std::runtime_error("Unable to determine the available moral alignments");
    }

    std::set<Adndtk::Defs::moral_alignment> aligns;

    auto clsId = static_cast<int>(cls);
    auto rsAlign = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_moral_alignments_by_class, clsId);
    for (auto& a : rsAlign)
    {
        auto alignId = static_cast<Defs::moral_alignment>(a.as<int>("alignment_id"));
        aligns.emplace(alignId);
    }
    return aligns;
}

std::set<Adndtk::Defs::moral_alignment> Adndtk::Cyclopedia::available_moral_alignments(const Defs::character_class& cls, const std::optional<Defs::deity>& deityId/*=std::nullopt*/) const
{
    if (cls == Defs::character_class::preist_of_specific_mythos)
    {
        return available_moral_alignments_by_mythos(cls, deityId);
    }

    if (!Cyclopedia::get_instance().is_multiclass(cls))
    {
        return available_moral_alignments_by_single_class(cls);
    }

    std::set<Adndtk::Defs::moral_alignment> alignments{
        Defs::moral_alignment::lawful_good,
        Defs::moral_alignment::lawful_neutral,
        Defs::moral_alignment::lawful_evil,
        Defs::moral_alignment::neutral_good,
        Defs::moral_alignment::true_neutral,
        Defs::moral_alignment::neutral_evil,
        Defs::moral_alignment::chaotic_good,
        Defs::moral_alignment::chaotic_neutral,
        Defs::moral_alignment::chaotic_evil
    };

    auto classes = Cyclopedia::get_instance().split<Defs::character_class>(cls);
    for (auto& c : classes)
    {
        auto availableAligns = available_moral_alignments_by_single_class(c);

        for (auto alignId : {
                        Defs::moral_alignment::lawful_good,
                        Defs::moral_alignment::lawful_neutral,
                        Defs::moral_alignment::lawful_evil,
                        Defs::moral_alignment::neutral_good,
                        Defs::moral_alignment::true_neutral,
                        Defs::moral_alignment::neutral_evil,
                        Defs::moral_alignment::chaotic_good,
                        Defs::moral_alignment::chaotic_neutral,
                        Defs::moral_alignment::chaotic_evil
                    })
        {
            if (availableAligns.find(alignId) == availableAligns.end())
            {
                alignments.erase(alignId);
            }
        }
    }
    return alignments;
}

std::set<Adndtk::Defs::deity> Adndtk::Cyclopedia::available_deities(const Defs::moral_alignment& align) const
{
    std::set<Adndtk::Defs::deity> deities;
    auto alignId = static_cast<int>(align);
    auto rsDeities = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_deities_by_moral_alignment, alignId);
    for (auto& d : rsDeities)
    {
        auto deityId = static_cast<Defs::deity>(d.as<int>("deity_id"));
        deities.emplace(deityId);
    }
        
    return deities;
}