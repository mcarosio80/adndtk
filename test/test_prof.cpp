//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-PROF.001] ", "[proficiencies]" )
{
    auto clsId{Defs::character_class::necromancer};
    ProficiencySlots slots{clsId};

    REQUIRE(slots.get_base_weapon_initial_score() == 1);
    REQUIRE(slots.get_base_weapon_num_levels() == 6);
    REQUIRE(slots.get_base_penalty() == -5);
    REQUIRE(slots.get_base_non_weapon_initial_score() == 4);
    REQUIRE(slots.get_base_non_weapon_num_levels() == 3);
}