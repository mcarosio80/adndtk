#include <cyclopedia.h>
#include <common_types.h>

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

        prepare_statement("select 'id', id, 'class_type_id', class_type_id, 'long_name', long_name, 'short_name', short_name, 'acronym', acronym from character_class where id = ?", Query::select_character_class);
        prepare_statement("select 'description', description, 'title_level', title_level, 'title', title, 'hit_dice', hit_dice, 'hp_after_title', hp_after_title from character_class_type where id = ?", Query::select_character_class_type);

        prepare_statement("select 'race_id', race_id, 'skill_id', skill_id, 'value', value from skill_modifier where race_id = ? and skill_id = ?", Query::select_skill_modifier);
        prepare_statement("select 'skill_value_required', skill_value_required from SCHOOL_OF_MAGIC where class_id = ?", Query::select_school_of_magic_skill_requisite);

        prepare_statement("select 'class_id', class_id, 'level', level, 'score', score from LEVEL_ADVANCEMENT order by class_id, level", Query::select_level_advancement);
        prepare_statement("select 'class_id', class_id, 'score', score from LEVEL_ADVANCEMENT_FACTOR", Query::select_level_advancement_factor);

        prepare_statement("select 'class_id', class_id, 'level_limit', level_limit from CLASS_AVAILABILITY where LEVEL_LIMIT is not null and CLASS_ID & ? and race_id = ?", Query::select_class_limits);

        prepare_statement("select 'class_type_id', sts.class_type_id, 'level', sts.level, 'saving_throw_id', st.id, 'score', sts.score from SAVING_THROW_SCORES sts inner join SAVING_THROW st on st.score = sts.SAVING_THROW_ID", Query::select_saving_throws);

        prepare_statement("select 'score', score, 'factor', factor from THACO t where CLASS_TYPE_ID = ?", Query::select_thaco);

        prepare_statement("select 'level', level, 'turnable_code', turnable_code, 'effect', effect, 'value', value from TURN_UNDEAD", Query::select_turn_undead);

        prepare_statement("select 'coin_from', coin_from, 'coin_to', coin_to, 'exchange_value', exchange_value from coin_exchange_values", Query::select_coin_exchange_rates);

        prepare_statement("select 'name', name, 'type', type, 'cost_coin', cost_coin, 'cost_min', cost_min, 'cost_max', cost_max, 'weight', weight, 'body_slot', body_slot, 'capacity_limit', capacity_limit from equipment e left join BODY_SLOT b on b.id = e.body_slot where e.id = ?", Query::select_equipment);

        prepare_statement("select 'id', id, 'ability_score_from', ability_score_from, 'excp_from', excp_from, 'ability_score_to', ability_score_to, 'excp_to', excp_to, 'hit_modifier', hit_modifier, 'damage_adjustment', damage_adjustment, 'weight_allowance', weight_allowance, 'maximum_press', maximum_press, 'open_doors', open_doors, 'open_doors_special', open_doors_special, 'bend_bars_lift_gates', bend_bars_lift_gates, 'note', note from strength_stats", Query::select_strength_stats);
        prepare_statement("select 'ability_score', ability_score, 'reaction_adjustment', reaction_adjustment, 'missile_attack_adjustment', missile_attack_adjustment, 'defensive_adjustment', defensive_adjustment from dexterity_stats", Query::select_dexterity_stats);
        prepare_statement("select 'ability_score', ability_score, 'hit_point_adjustment', hit_point_adjustment, 'hit_point_adjustment_warriors', hit_point_adjustment_warriors, 'rolls_upgrade', rolls_upgrade, 'system_shock', system_shock, 'resurrection_survival', resurrection_survival, 'poison_save', poison_save, 'regeneration_points', regeneration_points, 'regeneration_turns', regeneration_turns from constitution_stats", Query::select_constitution_stats);
        prepare_statement("select 'ability_score', ability_score, 'number_of_languages', number_of_languages, 'spell_level', spell_level, 'chance_to_learn_spell', chance_to_learn_spell, 'max_number_of_spells_per_level', max_number_of_spells_per_level, 'illusion_immunity', illusion_immunity from intelligence_stats", Query::select_intelligence_stats);
        prepare_statement("select 'ability_score', ability_score, 'magical_defence_adjustment', magical_defence_adjustment, 'bonus_spell_level_1', bonus_spell_level_1, 'bonus_spell_level_2', bonus_spell_level_2, 'bonus_spell_level_3', bonus_spell_level_3, 'bonus_spell_level_4', bonus_spell_level_4, 'bonus_spell_level_5', bonus_spell_level_5, 'bonus_spell_level_6', bonus_spell_level_6, 'bonus_spell_level_7', bonus_spell_level_7, 'chance_of_spell_failure', chance_of_spell_failure, 'spell_immunity', spell_immunity from wisdom_stats", Query::select_wisdom_stats);
        prepare_statement("select 'ability_score', ability_score, 'maximum_number_of_henchmen', maximum_number_of_henchmen, 'loyalty_base', loyalty_base, 'reaction_adjustment', reaction_adjustment from charisma_stats", Query::select_charisma_stats);

        prepare_statement("select 'id', id, 'base_score', base_score from thief_ability", Query::select_thief_ability_base_scores);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_armour_adjustments where armour_id = ?", Query::select_thieving_skill_armour_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_armour_adjustments where armour_id is null", Query::select_thieving_skill_no_armour_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_dexterity_adjustments where dexterity = ?", Query::select_thieving_skill_dexterity_adjustments);
        prepare_statement("select 'thieving_skill', thieving_skill, 'modifier', modifier from thieving_skill_racial_adjustments where race_id = ?", Query::select_thieving_skill_racial_adjustments);

        prepare_statement("select 'name', name, 'level', level from wizard_spell where id = ?", Query::select_wizard_spell);
        prepare_statement("select 'school_id', school_id from wizard_spell_school where spell_id = ?", Query::select_wizard_spell_school);
        prepare_statement("select 'school_id', school_id from school_of_magic_access where class_id = ? and school_id = ?", Query::select_school_of_magic_access);
        prepare_statement("select 'race_id', r.race_id from SCHOOL_OF_MAGIC_ACCESS a "
                            "inner join school_of_magic_per_races r on r.SCHOOL_ID = a.SCHOOL_ID "
                            "where a.CLASS_ID = ? and r.race_id = ? "
                            "group by r.race_id", Query::select_school_of_magic_per_race);
        prepare_statement("select 'level', level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4, 'spell_level_5', spell_level_5, 'spell_level_6', spell_level_6, 'spell_level_7', spell_level_7, 'spell_level_8', spell_level_8, 'spell_level_9', spell_level_9 from wizard_spell_progression where level = ?", Query::select_wizard_spell_progression);

        prepare_statement("select 'name', name, 'level', level from priest_spell where id = ?", Query::select_priest_spell);
        prepare_statement("select 'sphere_id', sphere_id from priest_spell_sphere where spell_id = ?", Query::select_priest_spell_sphere);
        prepare_statement("select 'sphere_id', sphere_id , 'access_level', access_level , 'access_mode', access_mode from sphere_access_per_class where class_id = ? and sphere_id = ?", Query::select_sphere_access_per_class);
        prepare_statement("select 'level', level, 'spell_level_1', spell_level_1, 'spell_level_2', spell_level_2, 'spell_level_3', spell_level_3, 'spell_level_4', spell_level_4, 'spell_level_5', spell_level_5, 'spell_level_6', spell_level_6, 'spell_level_7', spell_level_7 from priest_spell_progression where level = ?", Query::select_priest_spell_progression);

        prepare_statement("select 'id', ps.id, 'access_level', c.access_level, 'access_mode', c.access_mode from sphere_access_per_class c "
                            "inner join priest_spell_sphere s on s.sphere_id = c.sphere_id "
                            "inner join priest_spell ps on ps.id = s.spell_id "
                            "where c.class_id = ? and ps.level = ? "
                            "group by ps.id, c.access_level, c.access_mode", Query::select_priest_spells_per_class_level);
        
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