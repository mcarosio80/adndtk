//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-DICE.001] Rolling 3d8 results in (3 - 24)", "[dice]" )
{
    Die d{Defs::die::d6};
    int numRoll = 3;
    
    for (int i=0; i<6; ++i)
    {
        int result = d.roll(numRoll);
        REQUIRE(3 <= result);
        REQUIRE(result <= 24);
    }
}

TEST_CASE("[TC-DICE.002] Result of a dice expression (4d4+1) between (5 - 17)", "[dice][dice_regex]" )
{
    std::string expr{"4d4+1"};
    for (int i=0; i<6; ++i)
    {
        int result = Die::roll(expr.c_str());
        REQUIRE(5 <= result);
        REQUIRE(result <= 17);
    }
}

TEST_CASE("[TC-DICE.003] Non canonical dice expression parses correctly (3 D12 -   2) between (1 - 34)", "[dice][dice_regex]" )
{
    std::string expr{"3 D12 -   2"};
    for (int i=0; i<6; ++i)
    {
        int result = Die::roll(expr.c_str());
        REQUIRE(1 <= result);
        REQUIRE(result <= 34);
    }
}

TEST_CASE("[TC-DICE.004] Malformed dice expression throws runtime error", "[dice][dice_regex]" )
{
    REQUIRE_THROWS_AS(Die::roll("-3d4-"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("-5d4"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("3d6-"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("3d6+"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5e4+1"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5e4+f"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5e4-s"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("d6"), std::runtime_error);
}

TEST_CASE("[TC-DICE.005] Empty dice expression throws runtime error", "[dice][dice_regex]" )
{
    REQUIRE_THROWS_AS(Die::roll(""), std::runtime_error);
}

TEST_CASE("[TC-DICE.006] Non standard die type in dice expression is handled correctly", "[dice]" )
{
    int result = Die::roll("3d3");
    REQUIRE(3 <= result);
    REQUIRE(result <= 9);
}

TEST_CASE("[TC-DICE.007] Dice creation defaults to d6 when not specified", "[dice]" )
{
    Die dd;
    REQUIRE(dd.faces() == Defs::die::d6);
}

TEST_CASE("[TC-DICE.008] Rolling default die (d6) between (1 - 6)", "[dice]" )
{
    Die dd{};
    
    for (int i=0; i<6; ++i)
    {
        auto result = static_cast<int>(dd);
        REQUIRE(1 <= result);
        REQUIRE(result <= 6);
    }
}

TEST_CASE("[TC-DICE.009] Result of a range expression (3-8) between (3- 8)", "[dice][range_regex]" )
{
    for (int i=0; i<6; ++i)
    {
        int result = Die::roll("3 - 8");
        REQUIRE(3 <= result);
        REQUIRE(result <= 8);
    }
}

TEST_CASE("[TC-DICE.010] Result of a range expression (4  -  10) between (4-10)", "[dice][range_regex]" )
{
    for (int i=0; i<6; ++i)
    {
        int result = Die::roll("4  -  10");
        REQUIRE(4 <= result);
        REQUIRE(result <= 10);
    }
}

TEST_CASE("[TC-DICE.011] Malformed range expression throws runtime error", "[dice][range_regex]" )
{
    REQUIRE_THROWS_AS(Die::roll("- 4"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5-"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("-"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("r-7"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5-u+1"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("5-10+3"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("fhetuw"), std::runtime_error);
    REQUIRE_THROWS_AS(Die::roll("a-b"), std::runtime_error);
}

TEST_CASE("[TC-DICE.012] DiceSet stores static set of dice", "[dice]")
{
    for (short n = 1; n <= 3; ++n)
    {
        auto d4 = DiceSet::get_instance().roll(Defs::die::d4, n);
        REQUIRE(d4 >= 1 * n);
        REQUIRE(d4 <= 4 * n);
        auto d6 = DiceSet::get_instance().roll(Defs::die::d6, n);
        REQUIRE(d6 >= 1 * n);
        REQUIRE(d6 <= 6 * n);
        auto d8 = DiceSet::get_instance().roll(Defs::die::d8, n);
        REQUIRE(d8 >= 1 * n);
        REQUIRE(d8 <= 8 * n);
        auto d10 = DiceSet::get_instance().roll(Defs::die::d10, n);
        REQUIRE(d10 >= 1 * n);
        REQUIRE(d10 <= 10 * n);
        auto d12 = DiceSet::get_instance().roll(Defs::die::d12, n);
        REQUIRE(d12 >= 1 * n);
        REQUIRE(d12 <= 12 * n);
        auto d20 = DiceSet::get_instance().roll(Defs::die::d20, n);
        REQUIRE(d20 >= 1 * n);
        REQUIRE(d20 <= 20 * n);
        auto d100 = DiceSet::get_instance().roll(Defs::die::d100, n);
        REQUIRE(d100 >= 1 * n);
        REQUIRE(d100 <= 100 * n);
    }
}