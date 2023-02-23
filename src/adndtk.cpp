#include "adndtk.h"

#include <sstream>

bool Adndtk::Cyclopedia::_initialised = false;

Adndtk::Cyclopedia& Adndtk::Cyclopedia::get_instance()
{
    static Cyclopedia _instance;

    if (!_initialised)
    {
        _instance.init();
    }
    return _instance;
}

bool Adndtk::Cyclopedia::init()
{
    int ret = sqlite3_open_v2(Settings::database_path, &_dbConn, SQLITE_OPEN_READWRITE | SQLITE_OPEN_URI, NULL);
    
    return check_state(ret);
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
    if (errno != SQLITE_OK)
	{
        std::stringstream ss;
        ss << sqlite3_errstr(return_code) << ": " << sqlite3_errmsg(_dbConn);
        sqlite3_close_v2(_dbConn);
        ErrorManager::get_instance().error(ss.str().c_str());
        return false;
    }
    return true;
}