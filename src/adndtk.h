#pragma once

#include "../generated/defs.h"
#include "../generated/config.h"
#include <sqlite3.h>

namespace Adndtk
{
    class Cyclopedia
    {
    public:
        static Cyclopedia& get_instance();
        Cyclopedia(Cyclopedia const&) = delete;
        void operator=(Cyclopedia const&) = delete;

    private:
        Cyclopedia();
        ~Cyclopedia();

        bool init();
        bool check_state(int return_code);

		static bool     _initialised;
        sqlite3         *_dbConn;
    };
}