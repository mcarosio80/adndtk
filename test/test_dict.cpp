//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <dictionary.h>

using namespace Adndtk;

TEST_CASE("[TC-DICT.001] Calendar descriptions of enumeration values match the expectation", "[dictionary]" )
{
    REQUIRE(Dictionary::to_string(Defs::calendar::hapthos_calendar) == "Hapthos Calendar");
}

TEST_CASE("[TC-DICT.002] Race descriptions of enumeration values match the expectation", "[dictionary]" )
{
    REQUIRE(Dictionary::to_string(Defs::race::human) == "Human");
    REQUIRE(Dictionary::to_string(Defs::race::dwarf) == "Dwarf");
    REQUIRE(Dictionary::to_string(Defs::race::halfling) == "Halfling");
}

TEST_CASE("[TC-DICT.003] Deity descriptions of enumeration values match the expectation", "[dictionary]" )
{
    REQUIRE(Dictionary::to_string(Defs::deity::corellon_larethian) == "Corellon Larethian");
    REQUIRE(Dictionary::to_string(Defs::deity::berronar_truesilver) == "Berronar Truesilver");
    REQUIRE(Dictionary::to_string(Defs::deity::arvoreen_the_defender) == "Arvoreen the Defender");
    REQUIRE(Dictionary::to_string(Defs::deity::gargauth_the_outcast) == "Gargauth the Outcast");
    REQUIRE(Dictionary::to_string(Defs::deity::iyachtu_xvim) == "Iyachtu Xvim");
    REQUIRE(Dictionary::to_string(Defs::deity::gruumsh) == "Gruumsh");
}