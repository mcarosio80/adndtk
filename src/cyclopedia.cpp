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
        for (const auto& q : Queries)
        {
            prepare_statement(q.second.c_str(), q.first);
        }
        
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

bool Adndtk::Cyclopedia::can_have_exceptional_strength(const Defs::character_class& cls, const Defs::race& race, const Defs::skill& skillId, const short& skillValue) const
{
    return skillValue == 18 && skillId == Defs::skill::strength
        && Cyclopedia::get_instance().is_type_of<Defs::character_class_type::warrior>(cls)
        && race != Defs::race::halfling;
}

bool Adndtk::Cyclopedia::can_have_exceptional_strength(const Defs::character_class& cls, const Defs::race& race, const SkillValue& skillVal) const
{
    return can_have_exceptional_strength(cls, race, skillVal.type(), skillVal);
}

bool Adndtk::Cyclopedia::is_multiclass(const Defs::character_class& cls)
{
    return split<Defs::character_class>(cls).size() > 1;
}

std::optional<Adndtk::Defs::school_of_magic> Adndtk::Cyclopedia::get_school_of_magic(const Defs::character_class& cls)
{
    std::optional<Adndtk::Defs::school_of_magic> school{std::nullopt};
    auto classes = split<Defs::character_class>(cls);
    for (auto& clsId : classes)
    {
        auto setSet = exec_prepared_statement<Defs::character_class>(Query::select_school_of_magic_per_class, clsId);
        if (setSet.size() > 0)
        {
            auto& ret = setSet[0];
            school = ret.as<Defs::school_of_magic>("id") ;
        }
    }
    return school;
}

bool Adndtk::Cyclopedia::is_specialist_wizard(const Defs::character_class& cls)
{
    return get_school_of_magic(cls).has_value();
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
            (*set) += v;
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

    int rc = sqlite3_exec(_dbConn, stmt, Cyclopedia::onSqliteDataRetrieved, (void*)&set, &error);
    if (rc != SQLITE_OK)
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
        auto cls = r.as<Defs::character_class>("class_id");
        auto lvl = r.as<ExperienceLevel>("level");
        auto score = r.as<XP>("score");

        _advTable.add_level(cls, lvl, score);
    }

    auto advFactors = exec_prepared_statement<>(Query::select_level_advancement_factor);
    for (auto& r : advFactors)
    {
        auto cls = r.as<Defs::character_class>("class_id");
        auto score = r.as<XP>("score");

        _advTable.set_advancement_factor(cls, score);
    }
}

Adndtk::Defs::character_class_type Adndtk::Cyclopedia::get_class_type(const Defs::character_class& cls)
{
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(Adndtk::Query::select_character_class, cls);
    auto& clsInfo = rs[0];
    return clsInfo.as<Defs::character_class_type>("class_type_id");
}

std::vector<Adndtk::Defs::character_class_type> Adndtk::Cyclopedia::get_class_types(const Defs::character_class& cls)
{
    auto types = get_class_type(cls);
    return split<Defs::character_class_type>(types);
}