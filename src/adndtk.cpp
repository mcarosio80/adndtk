#include <adndtk.h>

#include <sstream>
#include <cstring>

bool Adndtk::Cyclopedia::_initialised = false;



Adndtk::Cyclopedia::QueryResult::QueryResult()
{
}

bool Adndtk::Cyclopedia::QueryResult::exists(const std::string& key) const
{
    auto value = (*this)[key];
    return value.has_value();
}

void Adndtk::Cyclopedia::QueryResult::add(const char *jsonKey, const rapidjson::Value& jsonValue)
{
    auto valType = jsonValue.GetType();
    std::stringstream ss;

    if (valType == rapidjson::kNullType)
    {
        add(jsonKey);
    }
    else if (valType == rapidjson::kNumberType)
    {
        if (jsonValue.IsUint())
        {
            ss << jsonValue.GetUint();
        }
        else if (jsonValue.IsInt())
        {
            ss << jsonValue.GetInt();
        }
        else if (jsonValue.IsUint64())
        {
            ss << jsonValue.GetUint64();
        }
        else if (jsonValue.IsInt64())
        {
            ss << jsonValue.GetInt64();
        }
        else if (jsonValue.IsDouble())
        {
            ss << jsonValue.GetDouble();
        }
        else if (jsonValue.IsFloat())
        {
            ss << jsonValue.GetFloat();
        }
        std::string value;
        ss >> value;
        add(jsonKey, value);
    }
    else if (valType == rapidjson::kStringType)
    {
        add(jsonKey, jsonValue.GetString());
    }
    else if (valType == rapidjson::kTrueType)
    {
        add(jsonKey, "true");
    }
    else if (valType == rapidjson::kFalseType)
    {
        add(jsonKey, "false");
    }
}

void Adndtk::Cyclopedia::QueryResult::add(const std::string& key, const std::string& value)
{
    _values[key] = value;
}

void Adndtk::Cyclopedia::QueryResult::add(const std::string& key)
{
    _values[key] = std::nullopt;
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

std::vector<Adndtk::Cyclopedia::QueryResult> Adndtk::Cyclopedia::from_json(const char *jsonText)
{
    rapidjson::Document jsonDoc;
    jsonDoc.Parse(jsonText);
    
    std::vector<Adndtk::Cyclopedia::QueryResult> set;
    for (rapidjson::Value::ConstValueIterator it = jsonDoc.Begin(); it != jsonDoc.End(); ++it)
    {
        auto record = it->GetObject();
        Adndtk::Cyclopedia::QueryResult result;
        for (auto& m : record)
        {
            auto key = m.name.GetString();
            result.add(key, m.value);
        }
        set.push_back(result);
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

std::vector<Adndtk::Cyclopedia::QueryResult> Adndtk::Cyclopedia::parse_json_result(sqlite3_stmt* stmt)
{
    std::vector<QueryResult> resultSet;
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        auto colValue = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        resultSet = from_json(colValue);
    }
    return resultSet;
}

std::vector<Adndtk::Cyclopedia::QueryResult> Adndtk::Cyclopedia::parse_tabular_result(sqlite3_stmt* stmt)
{
    int rowCount = 0;
    int columnCount = sqlite3_column_count(stmt);

    std::vector<std::string> captions;
    for (int col=0; col<columnCount; ++col)
    {
        std::string colName{sqlite3_column_name(stmt, col)};
        captions.push_back(colName);
    }

    std::vector<QueryResult> resultSet;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        QueryResult res;
        for (int col = 0; col<columnCount; ++col)
        {
            std::string colValue{reinterpret_cast<const char*>(sqlite3_column_text(stmt, col))};
            res.add(captions[col], colValue);
        }
        resultSet.push_back(res);
    }
    return resultSet;
}

int Adndtk::Cyclopedia::onSqliteDataRetrieved(void *data, int argc, char **argv, char **columnName)
{
    std::vector<QueryResult> *set = reinterpret_cast<std::vector<QueryResult>*>(data);

    if (Settings::query_type == QueryType::json)
    {
        for (int i=0; i<argc; ++i)
        {
            auto v = from_json(argv[i]);
            set->insert(set->end(), v.begin(), v.end());
        }
    }
    else if (Settings::query_type == QueryType::plain)
    {
        QueryResult res;
        for (int i=0; i<argc; ++i)
        {
            res.add(columnName[i], argv[i]);
        }
        set->push_back(res);
    }
    
    return 0;
}

std::vector<Adndtk::Cyclopedia::QueryResult> Adndtk::Cyclopedia::exec(const char* stmt)
{
    std::vector<QueryResult> set;
    char *error = nullptr;
    auto rc = sqlite3_exec(_dbConn, stmt, Cyclopedia::onSqliteDataRetrieved, (void*)&set, &error);

    if (rc != SQLITE_OK)
    {
        std::string msg{error};
        sqlite3_free(error);
        ErrorManager::get_instance().error(msg.c_str());
    }
    return set;
}