#include "adndtk.h"

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
    if (ret != SQLITE_OK)
	{
        const char *msg = sqlite3_errstr(ret);
		ErrorManager::get_instance().error(msg);
		return false;
	}
    
    return true;
}

Adndtk::Cyclopedia::Cyclopedia()
    : _dbConn(nullptr)
{
}

Adndtk::Cyclopedia::~Cyclopedia()
{
    int ret = sqlite3_close_v2(_dbConn);
    if (ret != SQLITE_OK)
	{
        const char *msg = sqlite3_errstr(ret);
		ErrorManager::get_instance().error(msg);
	}
}