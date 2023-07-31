//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-ENCU.001] Humans base movement factor equals 12", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::human) == 12);
}

TEST_CASE("[TC-ENCU.002] Elves base movement factor equals 12", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::elf) == 12);
}

TEST_CASE("[TC-ENCU.003] Dwarves base movement factor equals 6", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::dwarf) == 6);
}

TEST_CASE("[TC-ENCU.004] Gnomes base movement factor equals 6", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::gnome) == 6);
}

TEST_CASE("[TC-ENCU.005] Half-elves base movement factor equals 12", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::half_elf) == 12);
}

TEST_CASE("[TC-ENCU.006] Halflings base movement factor equals 6", "[encumbrance]" )
{
    REQUIRE(Encumbrance::get_instance().base_movement(Defs::race::halfling) == 6);
}

TEST_CASE("[TC-ENCU.007] Humans movement factor decreases when allowed weight increases", "[encumbrance]" )
{
    auto raceId = Defs::race::human;
    SkillValue strVal{Defs::skill::strength, 18, 72};
    double allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 60;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 80;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 10);
    allowedWeight = 95;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 8);
    allowedWeight = 110;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 5);
    allowedWeight = 120;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 3);
    allowedWeight = 130;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 137.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 140;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 13;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 30;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 9);
    allowedWeight = 45;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 5);
    allowedWeight = 55;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 60;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 63;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 63.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 64;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 5;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 11);
    allowedWeight = 6.2;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 8);
    allowedWeight = 7.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 5);
    allowedWeight = 9;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 9.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 10;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 11;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 12;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 17;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 38;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 12);
    allowedWeight = 50;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 9);
    allowedWeight = 69;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 85;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 3);
    allowedWeight = 90;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 95;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 98.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 100;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);
}

TEST_CASE("[TC-ENCU.008] Dwarves movement factor decreases when allowed weight increases", "[encumbrance]" )
{
    auto raceId = Defs::race::dwarf;
    SkillValue strVal{Defs::skill::strength, 18, 72};
    double allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 60;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 80;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 5);
    allowedWeight = 95;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 4);
    allowedWeight = 110;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 120;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 130;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 137.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 140;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 13;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 30;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 4);
    allowedWeight = 45;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 55;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 60;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 63;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 63.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 64;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 5;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 5);
    allowedWeight = 6.2;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 4);
    allowedWeight = 7.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 2);
    allowedWeight = 9;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 9.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 10;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 11;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 12;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);


    strVal = 17;
    allowedWeight = 0.0;

    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 38;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 6);
    allowedWeight = 50;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 4);
    allowedWeight = 69;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 3);
    allowedWeight = 85;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 90;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 95;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 98.5;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 1);
    allowedWeight = 100;
    REQUIRE(Encumbrance::get_instance().movement_factor(strVal, raceId, allowedWeight) == 0);
}