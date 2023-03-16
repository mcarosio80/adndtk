//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>


TEST_CASE("[TC-DICE] Dice rolls yield values between expected boudaries")
{
    SECTION("[TC-DICE.001] Rolling 3d8 results in (3 - 24)", "[dice]" )
    {
        Adndtk::Die d{Adndtk::Defs::die::d6};
        int numRoll = 3;
        
        for (int i=0; i<6; ++i)
        {
            int result = d.roll(numRoll);
            REQUIRE(3 <= result);
            REQUIRE(result <= 24);
        }
    }

    SECTION("[TC-DICE.002] Result of a dice expression (4d4+1) between (5 - 17)", "[dice][dice_regex]" )
    {
        std::string expr{"4d4+1"};
        for (int i=0; i<6; ++i)
        {
            int result = Adndtk::Die::roll(expr.c_str());
            REQUIRE(5 <= result);
            REQUIRE(result <= 17);
        }
    }

    SECTION("[TC-DICE.003] Non canonical dice expression parses correctly (3 D12 -   2) between (1 - 34)", "[dice][dice_regex]" )
    {
        std::string expr{"3 D12 -   2"};
        for (int i=0; i<6; ++i)
        {
            int result = Adndtk::Die::roll(expr.c_str());
            REQUIRE(1 <= result);
            REQUIRE(result <= 34);
        }
    }

    SECTION("[TC-DICE.004] Malformed dice expression throws runtime error", "[dice][dice_regex]" )
    {
        REQUIRE_THROWS_AS(Adndtk::Die::roll("-3d4-"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("-5d4"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("3d6-"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("3d6+"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5e4+1"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5e4+f"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5e4-s"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("d6"), std::runtime_error);
    }

    SECTION("[TC-DICE.005] Empty dice expression throws runtime error", "[dice][dice_regex]" )
    {
        REQUIRE_THROWS_AS(Adndtk::Die::roll(""), std::runtime_error);
    }

    SECTION("[TC-DICE.006] Non standard die type in dice expression is handled correctly", "[dice]" )
    {
        int result = Adndtk::Die::roll("3d3");
        REQUIRE(3 <= result);
        REQUIRE(result <= 9);
    }

    SECTION("[TC-DICE.007] Dice creation defaults to d6 when not specified", "[dice]" )
    {
        Adndtk::Die dd;
        REQUIRE(dd.faces() == Adndtk::Defs::die::d6);
    }

    SECTION("[TC-DICE.008] Rolling default die (d6) between (1 - 6)", "[dice]" )
    {
        Adndtk::Die dd;
        
        for (int i=0; i<6; ++i)
        {
            auto result = static_cast<int>(dd);
            REQUIRE(1 <= result);
            REQUIRE(result <= 6);
        }
    }

    SECTION("[TC-DICE.009] Result of a range expression (3-8) between (3- 8)", "[dice][range_regex]" )
    {
        for (int i=0; i<6; ++i)
        {
            int result = Adndtk::Die::roll("3 - 8");
            REQUIRE(3 <= result);
            REQUIRE(result <= 8);
        }
    }

    SECTION("[TC-DICE.010] Result of a range expression (4  -  10) between (4-10)", "[dice][range_regex]" )
    {
        for (int i=0; i<6; ++i)
        {
            int result = Adndtk::Die::roll("4  -  10");
            REQUIRE(4 <= result);
            REQUIRE(result <= 10);
        }
    }

    SECTION("[TC-DICE.011] Malformed range expression throws runtime error", "[dice][range_regex]" )
    {
        REQUIRE_THROWS_AS(Adndtk::Die::roll("- 4"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5-"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("-"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("r-7"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5-u+1"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("5-10+3"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("fhetuw"), std::runtime_error);
        REQUIRE_THROWS_AS(Adndtk::Die::roll("a-b"), std::runtime_error);
    }
}