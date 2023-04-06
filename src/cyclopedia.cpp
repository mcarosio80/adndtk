#include <cyclopedia.h>

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/allocators.h"

#include <sstream>
#include <cstring>

bool Adndtk::Cyclopedia::_initialised = false;



Adndtk::QueryResult::QueryResult()
{
}

bool Adndtk::QueryResult::exists(const std::string& key) const
{
    auto value = (*this)[key];
    return value.has_value();
}





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
        prepare_statement("select 'skill_value_required', skill_value_required from SCHOOL_OF_MAGIC where skill_id = ? and class_id = ?", Query::select_school_of_magic_skill_requisite);

        prepare_statement("select 'class_id', class_id, 'level', level, 'score', score from LEVEL_ADVANCEMENT order by class_id, level", Query::select_level_advancement);
        prepare_statement("select 'class_id', class_id, 'score', score from LEVEL_ADVANCEMENT_FACTOR", Query::select_level_advancement_factor);

        prepare_statement("select 'class_id', class_id, 'level_limit', level_limit from CLASS_AVAILABILITY where LEVEL_LIMIT is not null and CLASS_ID & ? and race_id = ?", Query::select_class_limits);

        prepare_statement("select 'class_type_id', sts.class_type_id, 'level', sts.level, 'saving_throw_id', st.id, 'score', sts.score from SAVING_THROW_SCORES sts inner join SAVING_THROW st on st.score = sts.SAVING_THROW_ID", Query::select_saving_throws);

        prepare_statement("select 'score', score, 'factor', factor from THACO t where CLASS_TYPE_ID = ?", Query::select_thaco);

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