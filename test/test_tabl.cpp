//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <defs.h>
#include <tables.h>

using namespace Adndtk;

TEST_CASE("[TC-TABL.001] ", "[tables]" )
{
    auto turnable_code{Defs::turnable::shadow};
    ExperienceLevel level{5};
    auto vals = Tables::turn_undead::select_by(turnable_code, level);
    REQUIRE(vals.size() == 1);
    REQUIRE(vals[0].effect == Defs::turn_effect::percentage);
    REQUIRE(vals[0].value == 7);
}