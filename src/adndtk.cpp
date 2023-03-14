#include <adndtk.h>

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

bool Adndtk::Cyclopedia::init()
{
    int ret = sqlite3_open_v2(Settings::database_path, &_dbConn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
    
    bool ok = check_state(ret);
    if (ok)
    {
        //select json_group_array(json_array(ID, ACRONYM, DESCRIPTION)) from coin
        prepare_statement("select id, acronym, description from coin where id = ?", Query::select_coin), QueryType::plain;
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
        selectStmt.replace(it, token.length(), "select json_group_array(json_array(");
        
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