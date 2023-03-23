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
    Die dd;
    
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

TEST_CASE("[TC-SKIL.001] Skills manually assigned reflect the expected values", "[skills]" )
{
    SkillValue skl{Defs::skill::dexterity, 13};
    REQUIRE_FALSE(skl.hasExceptionalStrength());
    REQUIRE(skl == 13);
    REQUIRE_FALSE(skl == 15);
    REQUIRE(skl != 8);
    REQUIRE_FALSE(skl != 13);
    REQUIRE(skl <= 13);
    REQUIRE(skl <= 18);
    REQUIRE(skl >= 13);
    REQUIRE(skl >= 11);
    REQUIRE_FALSE(skl < 13);
    REQUIRE(skl < 18);
    REQUIRE_FALSE(skl > 13);
}

TEST_CASE("[TC-SKIL.002] Comparison between different skills throws exceptions", "[skills]" )
{
    SkillValue dex{Defs::skill::dexterity, 13};
    SkillValue con{Defs::skill::constitution, 13};

    REQUIRE_THROWS(dex == con);
    REQUIRE_THROWS(dex != con);
    REQUIRE_THROWS(dex < con);
    REQUIRE_THROWS(dex <= con);
    REQUIRE_THROWS(dex >= con);
    REQUIRE_THROWS(dex > con);
}

TEST_CASE("[TC-SKIL.003] Comparison between omogeneous skills works as expected", "[skills]" )
{
    SkillValue sk1{Defs::skill::dexterity, 13};
    SkillValue sk2{Defs::skill::dexterity, 15};

    REQUIRE_FALSE(sk1 == sk2);
    REQUIRE(sk1 != sk2);
    REQUIRE(sk1 < sk2);
    REQUIRE(sk1 <= sk2);
    REQUIRE_FALSE(sk1 >= sk2);
    REQUIRE_FALSE(sk1 > sk2); 
}

TEST_CASE("[TC-SKIL.004] Comparison between exceptional strength values works as expected", "[skills]" )
{
    SkillValue sk1{Defs::skill::strength, 18, 34};
    SkillValue sk2{Defs::skill::strength, 18, 72};

    REQUIRE_FALSE(sk1 == sk2);
    REQUIRE(sk1 != sk2);
    REQUIRE(sk1 < sk2);
    REQUIRE(sk1 <= sk2);
    REQUIRE_FALSE(sk1 >= sk2);
    REQUIRE_FALSE(sk1 > sk2); 
}

TEST_CASE("[TC-SKIL.005] Comparison between exceptional strength value and non-exceptional strength value works as expected", "[skills]" )
{
    SkillValue sk1{Defs::skill::strength, 16};
    SkillValue sk2{Defs::skill::strength, 18, 49};

    REQUIRE_FALSE(sk1 == sk2);
    REQUIRE(sk1 != sk2);
    REQUIRE(sk1 < sk2);
    REQUIRE(sk1 <= sk2);
    REQUIRE_FALSE(sk1 >= sk2);
    REQUIRE_FALSE(sk1 > sk2); 
}

TEST_CASE("[TC-SKIL.006] Skills increment/decrement reflects the expected values", "[skills]" )
{
    SkillValue skl{Defs::skill::dexterity, 13};
    
    skl += 1;
    REQUIRE(skl == 14);
    skl += 2;
    REQUIRE(skl == 16);
    
    skl -= 4;
    REQUIRE(skl == 12);
    skl -= 1;
    REQUIRE(skl == 11);
}

TEST_CASE("[TC-SKIL.007] Exceptional strength increment/decrement reflects the expected values", "[skills]" )
{
    SkillValue str{Defs::skill::strength, 18, 25};
    
    REQUIRE(str == 18);
    REQUIRE(str.hasExceptionalStrength());
    REQUIRE(str.exceptionalStrength() == 25);
    str += 1;
    REQUIRE(str == 19);
    REQUIRE_FALSE(str.hasExceptionalStrength());
    REQUIRE(str.exceptionalStrength() == std::nullopt);
    str -= 1;
    REQUIRE(str == 18);
    REQUIRE(str.hasExceptionalStrength());
    REQUIRE(str.exceptionalStrength() == 25);
    str -= 1;
    REQUIRE(str == 17);
    REQUIRE_FALSE(str.hasExceptionalStrength());
    REQUIRE(str.exceptionalStrength() == std::nullopt);
    str += 1;
    REQUIRE(str == 18);
    REQUIRE(str.hasExceptionalStrength());
    REQUIRE(str.exceptionalStrength() == 25);
}

TEST_CASE("[TC-SKIL.008] Skills other than strength cannot have exceptional value", "[skills]" )
{
    REQUIRE_THROWS(SkillValue(Defs::skill::dexterity, 18, 52));
    REQUIRE_THROWS(SkillValue(Defs::skill::constitution, 18, 45));
    REQUIRE_THROWS(SkillValue(Defs::skill::intelligence, 18, 26));
    REQUIRE_THROWS(SkillValue(Defs::skill::wisdom, 18, 87));
    REQUIRE_THROWS(SkillValue(Defs::skill::charisma, 18, 91));
}

TEST_CASE("[TC-SKIL.009] Strength less than 18 cannot have exceptional value", "[skills]" )
{
    REQUIRE_THROWS(SkillValue(Defs::skill::strength, 16, 46));
    REQUIRE_THROWS(SkillValue(Defs::skill::strength, 19, 65));
}

TEST_CASE("[TC-SKIL.010] Exceptional strength value cannot exceed the range 1 - 100", "[skills]" )
{
    REQUIRE_THROWS(SkillValue(Defs::skill::strength, 18, 120));
    REQUIRE_THROWS(SkillValue(Defs::skill::strength, 18, -3));
}

TEST_CASE("[TC-SKIL.011] Test on exceptional strength value respond as expected", "[skills]" )
{
    SkillValue skl{Defs::skill::strength, 18, 71};
    REQUIRE(skl.hasExceptionalStrength());
}

TEST_CASE("[TC-SKIL.012] Skills creation for human fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.013] Skills creation for human cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.014] Skills creation for human mage (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.015] Skills creation for human thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.016] Skills creation for human ranger (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::ranger;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 13);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 13);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 14);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 14);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.017] Skills creation for human paladin (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::paladin;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 12);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 9);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 13);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 17);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.018] Skills creation for human druid (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 12);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.019] Skills creation for human bard (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::bard;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 12);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 13);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.020] Skills creation for human preist_of_specific_mythos (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::preist_of_specific_mythos;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 3);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}