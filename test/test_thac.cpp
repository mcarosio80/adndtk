//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <defs.h>
#include <thaco.h>
#include <cyclopedia.h>

using namespace Adndtk;

TEST_CASE("[TC-THAC.001] THAC0 values for warrior improve according to the experience level", "[THAC0]" )
{
    Defs::character_class_type typ = Defs::character_class_type::warrior;
    Thaco t{typ};

    REQUIRE(t.get(1) == 20);
    REQUIRE(t.get(2) == 19);
    REQUIRE(t.get(3) == 18);
    REQUIRE(t.get(4) == 17);
    REQUIRE(t.get(5) == 16);
    REQUIRE(t.get(6) == 15);
    REQUIRE(t.get(7) == 14);
    REQUIRE(t.get(8) == 13);
    REQUIRE(t.get(9) == 12);
    REQUIRE(t.get(10) == 11);
    REQUIRE(t.get(11) == 10);
    REQUIRE(t.get(12) == 9);
    REQUIRE(t.get(13) == 8);
    REQUIRE(t.get(14) == 7);
    REQUIRE(t.get(15) == 6);
    REQUIRE(t.get(16) == 5);
    REQUIRE(t.get(17) == 4);
    REQUIRE(t.get(18) == 3);
    REQUIRE(t.get(19) == 2);
    REQUIRE(t.get(20) == 1);
}

TEST_CASE("[TC-THAC.002] THAC0 values for wizard improve according to the experience level", "[THAC0]" )
{
    Defs::character_class_type typ = Defs::character_class_type::wizard;
    Thaco t{typ};

    REQUIRE(t.get(1) == 20);
    REQUIRE(t.get(2) == 20);
    REQUIRE(t.get(3) == 20);
    REQUIRE(t.get(4) == 19);
    REQUIRE(t.get(5) == 19);
    REQUIRE(t.get(6) == 19);
    REQUIRE(t.get(7) == 18);
    REQUIRE(t.get(8) == 18);
    REQUIRE(t.get(9) == 18);
    REQUIRE(t.get(10) == 17);
    REQUIRE(t.get(11) == 17);
    REQUIRE(t.get(12) == 17);
    REQUIRE(t.get(13) == 16);
    REQUIRE(t.get(14) == 16);
    REQUIRE(t.get(15) == 16);
    REQUIRE(t.get(16) == 15);
    REQUIRE(t.get(17) == 15);
    REQUIRE(t.get(18) == 15);
    REQUIRE(t.get(19) == 14);
    REQUIRE(t.get(20) == 14);
}

TEST_CASE("[TC-THAC.003] THAC0 values for priest improve according to the experience level", "[THAC0]" )
{
    Defs::character_class_type typ = Defs::character_class_type::priest;
    Thaco t{typ};

    REQUIRE(t.get(1) == 20);
    REQUIRE(t.get(2) == 20);
    REQUIRE(t.get(3) == 20);
    REQUIRE(t.get(4) == 18);
    REQUIRE(t.get(5) == 18);
    REQUIRE(t.get(6) == 18);
    REQUIRE(t.get(7) == 16);
    REQUIRE(t.get(8) == 16);
    REQUIRE(t.get(9) == 16);
    REQUIRE(t.get(10) == 14);
    REQUIRE(t.get(11) == 14);
    REQUIRE(t.get(12) == 14);
    REQUIRE(t.get(13) == 12);
    REQUIRE(t.get(14) == 12);
    REQUIRE(t.get(15) == 12);
    REQUIRE(t.get(16) == 10);
    REQUIRE(t.get(17) == 10);
    REQUIRE(t.get(18) == 10);
    REQUIRE(t.get(19) == 8);
    REQUIRE(t.get(20) == 8);
}

TEST_CASE("[TC-THAC.004] THAC0 values for rogue improve according to the experience level", "[THAC0]" )
{
    Defs::character_class_type typ = Defs::character_class_type::rogue;
    Thaco t{typ};

    REQUIRE(t.get(1) == 20);
    REQUIRE(t.get(2) == 20);
    REQUIRE(t.get(3) == 19);
    REQUIRE(t.get(4) == 19);
    REQUIRE(t.get(5) == 18);
    REQUIRE(t.get(6) == 18);
    REQUIRE(t.get(7) == 17);
    REQUIRE(t.get(8) == 17);
    REQUIRE(t.get(9) == 16);
    REQUIRE(t.get(10) == 16);
    REQUIRE(t.get(11) == 15);
    REQUIRE(t.get(12) == 15);
    REQUIRE(t.get(13) == 14);
    REQUIRE(t.get(14) == 14);
    REQUIRE(t.get(15) == 13);
    REQUIRE(t.get(16) == 13);
    REQUIRE(t.get(17) == 12);
    REQUIRE(t.get(18) == 12);
    REQUIRE(t.get(19) == 11);
    REQUIRE(t.get(20) == 11);
}

TEST_CASE("[TC-THAC.005] Best THAC0 will be chosen for multiclass characters", "[THAC0]" )
{
    Defs::character_class cls{Defs::character_class::fighter_mage};
    auto classes = Cyclopedia::get_instance().split<Defs::character_class>(cls);
    CharacterExperience exp{cls};

    auto type = Cyclopedia::get_instance().get_class_type(cls);
    Thaco t{type};

    for (auto& c : classes)
    {
        exp.set_xp(c, 4000);
    }
    auto attackLvl = t.attack_as(exp);
    REQUIRE(attackLvl.first == Defs::character_class_type::warrior);
    REQUIRE(attackLvl.second == 3);

    exp.set_xp(Defs::character_class::fighter, 1000);
    exp.set_xp(Defs::character_class::mage, 10000);

    attackLvl = t.attack_as(exp);
    REQUIRE(attackLvl.first == Defs::character_class_type::wizard);
    REQUIRE(attackLvl.second == 4);
}

TEST_CASE("[TC-THAC.006] Creatures with less than 1 HD have THAC0 20", "[THAC0]" )
{
    REQUIRE(Thaco::get_value(0.25) == Const::base_thaco);
    REQUIRE(Thaco::get_value(0.25, 1) == Const::base_thaco);
    REQUIRE(Thaco::get_value(0.5) == Const::base_thaco);
    REQUIRE(Thaco::get_value(0.5, 1) == Const::base_thaco);
    REQUIRE(Thaco::get_value(1, -1) == Const::base_thaco);
}

TEST_CASE("[TC-THAC.007] THAC0 for creatures with 1+ or more HD decreases accordingly", "[THAC0]" )
{
    short currentThaco{Const::base_thaco - 1};

    for (HitDice hdValue{1}; hdValue < 20; hdValue += 2, currentThaco -= 2)
    {
        REQUIRE(Thaco::get_value(hdValue) == currentThaco);
        REQUIRE(Thaco::get_value(hdValue, 1) == currentThaco);
        REQUIRE(Thaco::get_value(hdValue + 1) == currentThaco);
        REQUIRE(Thaco::get_value(hdValue + 1, 1) == currentThaco);
    }
}