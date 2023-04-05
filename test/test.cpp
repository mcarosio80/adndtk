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

TEST_CASE("[TC-SKIL.021] Skills creation for human abjurer (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::abjurer;
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
    REQUIRE(wis >= 15);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.022] Skills creation for human conjurer (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::conjurer;
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
    REQUIRE(inl >= 15);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.023] Skills creation for human diviner (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::diviner;
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
    REQUIRE(wis >= 16);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.024] Skills creation for human enchanter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::enchanter;
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
    REQUIRE(cha >= 16);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.025] Skills creation for human illusionist (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::illusionist;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 16);
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

TEST_CASE("[TC-SKIL.026] Skills creation for human invoker (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::invoker;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 16);
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

TEST_CASE("[TC-SKIL.027] Skills creation for human necromancer (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::necromancer;
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
    REQUIRE(wis >= 16);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.028] Skills creation for human transmuter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::transmuter;
    Defs::race race = Defs::race::human;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 15);
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

TEST_CASE("[TC-SKIL.029] Skills creation for elf fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 7);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.030] Skills creation for elf ranger (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::ranger;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 13);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 14);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 13);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 14);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.031] Skills creation for elf ranger (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 7);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.032] Skills creation for elf fighter_mage (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 7);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.033] Skills creation for elf cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 7);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.034] Skills creation for elf thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.035] Skills creation for elf fighter_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_thief;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.036] Skills creation for elf mage_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage_thief;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 8);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.037] Skills creation for elf fighter_mage_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.037] Skills creation for elf diviner (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::diviner;
    Defs::race race = Defs::race::elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 7);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 17);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 16);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 8);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.038] Skills creation for dwarf fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::dwarf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 12);
    REQUIRE(con <= 19);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 2);
    REQUIRE(cha <= 17);
}

TEST_CASE("[TC-SKIL.039] Skills creation for dwarf cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::dwarf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 8);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 12);
    REQUIRE(con <= 19);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 2);
    REQUIRE(cha <= 17);
}

TEST_CASE("[TC-SKIL.040] Skills creation for dwarf thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::dwarf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 8);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 12);
    REQUIRE(con <= 19);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 2);
    REQUIRE(cha <= 17);
}

TEST_CASE("[TC-SKIL.041] Skills creation for dwarf fighter_cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_cleric;
    Defs::race race = Defs::race::dwarf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 12);
    REQUIRE(con <= 19);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 2);
    REQUIRE(cha <= 17);
}

TEST_CASE("[TC-SKIL.042] Skills creation for dwarf fighter_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_thief;
    Defs::race race = Defs::race::dwarf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 12);
    REQUIRE(con <= 19);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 3);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 2);
    REQUIRE(cha <= 17);
}

TEST_CASE("[TC-SKIL.043] Skills creation for gnome fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.044] Skills creation for gnome fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::illusionist;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 16);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 10);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.045] Skills creation for gnome cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 8);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.046] Skills creation for gnome thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.047] Skills creation for gnome fighter_illusionist (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_illusionist;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 16);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.048] Skills creation for gnome fighter_cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_cleric;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 3);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 6);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 8);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.049] Skills creation for gnome cleric_illusionist (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric_illusionist;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 16);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 8);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.050] Skills creation for gnome fighter_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_thief;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.051] Skills creation for gnome illusionist_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::illusionist_thief;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 16);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 2);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.052] Skills creation for gnome cleric_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric_thief;
    Defs::race race = Defs::race::gnome;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 8);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 7);
    REQUIRE(inl <= 19);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 8);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.053] Skills creation for half_elf fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.054] Skills creation for half_elf ranger (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::ranger;
    Defs::race race = Defs::race::half_elf;

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
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 14);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.055] Skills creation for half_elf mage (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
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

TEST_CASE("[TC-SKIL.056] Skills creation for half_elf fighter_mage (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.057] Skills creation for half_elf conjurer (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::conjurer;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 15);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.058] Skills creation for half_elf diviner (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::diviner;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 16);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.059] Skills creation for half_elf enchanter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::enchanter;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 16);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.060] Skills creation for half_elf transmuter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::transmuter;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 15);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
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

TEST_CASE("[TC-SKIL.061] Skills creation for half_elf cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.062] Skills creation for half_elf fighter_cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_cleric;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.063] Skills creation for half_elf cleric_ranger (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric_ranger;
    Defs::race race = Defs::race::half_elf;

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
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 14);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.064] Skills creation for half_elf mage_cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage_cleric;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.065] Skills creation for half_elf fighter_mage_cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_cleric;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.066] Skills creation for half_elf druid (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 12);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.067] Skills creation for half_elf fighter_druid (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_druid;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 12);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.068] Skills creation for half_elf mage_druid (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage_druid;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 12);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.069] Skills creation for half_elf fighter_mage_druid (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_druid;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 6);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 9);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 12);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 15);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.070] Skills creation for half_elf thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.071] Skills creation for half_elf fighter_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_thief;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 4);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 18);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.072] Skills creation for half_elf mage_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::mage_thief;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
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

TEST_CASE("[TC-SKIL.073] Skills creation for half_elf fighter_mage_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 9);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 9);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
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

TEST_CASE("[TC-SKIL.074] Skills creation for half_elf bard (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::bard;
    Defs::race race = Defs::race::half_elf;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 3);
    REQUIRE(str <= 18);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 12);
    REQUIRE(dex <= 18);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 6);
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

TEST_CASE("[TC-SKIL.075] Skills creation for halfling fighter (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    Defs::race race = Defs::race::halfling;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 8);
    REQUIRE(str <= 17);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 8);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 10);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 6);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.076] Skills creation for halfling cleric (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::cleric;
    Defs::race race = Defs::race::halfling;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 17);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 8);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 10);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 6);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 9);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.077] Skills creation for halfling thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::thief;
    Defs::race race = Defs::race::halfling;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 6);
    REQUIRE(str <= 17);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 10);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 6);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-SKIL.078] Skills creation for halfling fighter_thief (standard method) respects the class/race boundaries", "[skills][creation]" )
{
    Defs::character_class cls = Defs::character_class::fighter_thief;
    Defs::race race = Defs::race::halfling;

    SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
    REQUIRE(str >= 8);
    REQUIRE(str <= 17);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
    REQUIRE(dex >= 10);
    REQUIRE(dex <= 19);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
    REQUIRE(con >= 10);
    REQUIRE(con <= 18);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
    REQUIRE(inl >= 6);
    REQUIRE(inl <= 18);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
    REQUIRE(wis >= 3);
    REQUIRE(wis <= 17);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
    REQUIRE(cha >= 3);
    REQUIRE(cha <= 18);
}

TEST_CASE("[TC-ADVT.001] Advancement table for fighter contains expected values", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
    REQUIRE(advTable.get_xp_for_level(cls, 2) == 2000);
    REQUIRE(advTable.get_xp_for_level(cls, 3) == 4000);
    REQUIRE(advTable.get_xp_for_level(cls, 4) == 8000);
    REQUIRE(advTable.get_xp_for_level(cls, 5) == 16000);
    REQUIRE(advTable.get_xp_for_level(cls, 6) == 32000);
    REQUIRE(advTable.get_xp_for_level(cls, 7) == 64000);
    REQUIRE(advTable.get_xp_for_level(cls, 8) == 125000);
    REQUIRE(advTable.get_xp_for_level(cls, 9) == 250000);
    REQUIRE(advTable.get_xp_for_level(cls, 10) == 500000);
    REQUIRE(advTable.get_xp_for_level(cls, 11) == 750000);
    REQUIRE(advTable.get_xp_for_level(cls, 12) == 1000000);
    REQUIRE(advTable.get_xp_for_level(cls, 13) == 1250000);
    REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
    REQUIRE(advTable.get_xp_for_level(cls, 15) == 1750000);
    REQUIRE(advTable.get_xp_for_level(cls, 16) == 2000000);
    REQUIRE(advTable.get_xp_for_level(cls, 17) == 2250000);
    REQUIRE(advTable.get_xp_for_level(cls, 18) == 2500000);
    REQUIRE(advTable.get_xp_for_level(cls, 19) == 2750000);
    REQUIRE(advTable.get_xp_for_level(cls, 20) == 3000000);
}

TEST_CASE("[TC-ADVT.002] Advancement table for ranger/paladin contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::ranger,
        Defs::character_class::paladin
    };
    auto advTable = Adndtk::Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 2250);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 4500);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 9000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 18000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 36000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 75000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 150000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 300000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 600000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 900000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 1200000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1500000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1800000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 2100000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 2400000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2700000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 3000000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 3300000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 3600000);
    }
}

TEST_CASE("[TC-ADVT.003] Advancement table for mage/specialist contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::mage,
        Defs::character_class::abjurer,
        Defs::character_class::conjurer,
        Defs::character_class::diviner,
        Defs::character_class::enchanter,
        Defs::character_class::illusionist,
        Defs::character_class::abjurer,
        Defs::character_class::invoker,
        Defs::character_class::necromancer,
        Defs::character_class::transmuter
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 2500);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 5000);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 10000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 20000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 40000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 60000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 90000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 135000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 250000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 375000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 750000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1125000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1875000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 2250000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2625000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 3000000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 3375000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 3750000);
    }
}

TEST_CASE("[TC-ADVT.004] Advancement table for cleric/preist_of_specific_mythos contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::cleric,
        Defs::character_class::preist_of_specific_mythos,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 1500);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 3000);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 6000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 13000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 27500);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 55000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 110000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 225000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 450000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 675000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 900000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1125000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1350000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1575000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 1800000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2025000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 2250000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 2475000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 2700000);
    }
}

TEST_CASE("[TC-ADVT.005] Advancement table for druid contains expected values", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
    REQUIRE(advTable.get_xp_for_level(cls, 2) == 2000);
    REQUIRE(advTable.get_xp_for_level(cls, 3) == 4000);
    REQUIRE(advTable.get_xp_for_level(cls, 4) == 7500);
    REQUIRE(advTable.get_xp_for_level(cls, 5) == 12500);
    REQUIRE(advTable.get_xp_for_level(cls, 6) == 20000);
    REQUIRE(advTable.get_xp_for_level(cls, 7) == 35000);
    REQUIRE(advTable.get_xp_for_level(cls, 8) == 60000);
    REQUIRE(advTable.get_xp_for_level(cls, 9) == 90000);
    REQUIRE(advTable.get_xp_for_level(cls, 10) == 125000);
    REQUIRE(advTable.get_xp_for_level(cls, 11) == 200000);
    REQUIRE(advTable.get_xp_for_level(cls, 12) == 300000);
    REQUIRE(advTable.get_xp_for_level(cls, 13) == 750000);
    REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
    REQUIRE(advTable.get_xp_for_level(cls, 15) == 3000000);
    REQUIRE(advTable.get_xp_for_level(cls, 16) == 3500000);
    REQUIRE(advTable.get_xp_for_level(cls, 17) == 4000000);
    REQUIRE(advTable.get_xp_for_level(cls, 18) == 4500000);
    REQUIRE(advTable.get_xp_for_level(cls, 19) == 5000000);
    REQUIRE(advTable.get_xp_for_level(cls, 20) == 5500000);
}

TEST_CASE("[TC-ADVT.006] Advancement table for thief/bard contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::thief,
        Defs::character_class::bard,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 1250);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 2500);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 5000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 10000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 20000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 40000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 70000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 110000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 160000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 220000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 440000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 660000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 880000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1100000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 1320000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 1540000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 1760000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 1980000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 2200000);
    }
}

TEST_CASE("[TC-ADVT.007] Level lookup for fighter yields expected value", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_level(cls, 0) == 1);
    REQUIRE(advTable.get_level(cls, 1900) == 1);
    REQUIRE(advTable.get_level(cls, 2000) == 2);
    REQUIRE(advTable.get_level(cls, 7999) == 3);
    REQUIRE(advTable.get_level(cls, 8000) == 4);
    REQUIRE(advTable.get_level(cls, 200000) == 8);
    REQUIRE(advTable.get_level(cls, 500001) == 10);
    REQUIRE(advTable.get_level(cls, 1000100) == 12);
    REQUIRE(advTable.get_level(cls, 2000010) == 16);
    REQUIRE(advTable.get_level(cls, 3000000) == 20);
    REQUIRE(advTable.get_level(cls, 3400000) == 21);
}

TEST_CASE("[TC-ADVT.008] Level lookup for ranger/paladin yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::ranger,
        Defs::character_class::paladin,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1900) == 1);
        REQUIRE(advTable.get_level(cls, 2250) == 2);
        REQUIRE(advTable.get_level(cls, 8999) == 3);
        REQUIRE(advTable.get_level(cls, 9000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 8);
        REQUIRE(advTable.get_level(cls, 500001) == 9);
        REQUIRE(advTable.get_level(cls, 1000100) == 11);
        REQUIRE(advTable.get_level(cls, 2000010) == 14);
        REQUIRE(advTable.get_level(cls, 3000000) == 18);
        REQUIRE(advTable.get_level(cls, 3400000) == 19);
        REQUIRE(advTable.get_level(cls, 3600000) == 20);
        REQUIRE(advTable.get_level(cls, 3700000) == 20);
        REQUIRE(advTable.get_level(cls, 3900000) == 21);
    }
}

TEST_CASE("[TC-ADVT.009] Level lookup for mage/specialist yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::mage,
        Defs::character_class::abjurer,
        Defs::character_class::conjurer,
        Defs::character_class::diviner,
        Defs::character_class::enchanter,
        Defs::character_class::illusionist,
        Defs::character_class::invoker,
        Defs::character_class::necromancer,
        Defs::character_class::transmuter
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1900) == 1);
        REQUIRE(advTable.get_level(cls, 2500) == 2);
        REQUIRE(advTable.get_level(cls, 9999) == 3);
        REQUIRE(advTable.get_level(cls, 10000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 9);
        REQUIRE(advTable.get_level(cls, 500001) == 11);
        REQUIRE(advTable.get_level(cls, 1000100) == 12);
        REQUIRE(advTable.get_level(cls, 2000010) == 15);
        REQUIRE(advTable.get_level(cls, 3000000) == 18);
        REQUIRE(advTable.get_level(cls, 3400000) == 19);
        REQUIRE(advTable.get_level(cls, 3600000) == 19);
        REQUIRE(advTable.get_level(cls, 3750000) == 20);
        REQUIRE(advTable.get_level(cls, 4100000) == 20);
        REQUIRE(advTable.get_level(cls, 4200000) == 21);
    }
}

TEST_CASE("[TC-ADVT.010] Level lookup for cleric/preist_of_specific_mythos yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::cleric,
        Defs::character_class::preist_of_specific_mythos
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1400) == 1);
        REQUIRE(advTable.get_level(cls, 1500) == 2);
        REQUIRE(advTable.get_level(cls, 5999) == 3);
        REQUIRE(advTable.get_level(cls, 6000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 8);
        REQUIRE(advTable.get_level(cls, 500001) == 10);
        REQUIRE(advTable.get_level(cls, 1000100) == 12);
        REQUIRE(advTable.get_level(cls, 2000010) == 16);
        REQUIRE(advTable.get_level(cls, 2030000) == 17);
        REQUIRE(advTable.get_level(cls, 2500000) == 19);
        REQUIRE(advTable.get_level(cls, 2700000) == 20);
        REQUIRE(advTable.get_level(cls, 2924000) == 20);
        REQUIRE(advTable.get_level(cls, 2925000) == 21);
        REQUIRE(advTable.get_level(cls, 3000000) == 21);
    }
}

TEST_CASE("[TC-ADVT.010] Level lookup for druid yields expected value", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_level(cls, 0) == 1);
    REQUIRE(advTable.get_level(cls, 1999) == 1);
    REQUIRE(advTable.get_level(cls, 2000) == 2);
    REQUIRE(advTable.get_level(cls, 5999) == 3);
    REQUIRE(advTable.get_level(cls, 7500) == 4);
    REQUIRE(advTable.get_level(cls, 200000) == 11);
    REQUIRE(advTable.get_level(cls, 500001) == 12);
    REQUIRE(advTable.get_level(cls, 1000100) == 13);
    REQUIRE(advTable.get_level(cls, 2925000) == 14);
    REQUIRE(advTable.get_level(cls, 3000000) == 15);
    REQUIRE(advTable.get_level(cls, 3499999) == 15);
    REQUIRE(advTable.get_level(cls, 3500000) == 16);
    REQUIRE(advTable.get_level(cls, 3999999) == 16);
    REQUIRE(advTable.get_level(cls, 4000000) == 17);
    REQUIRE(advTable.get_level(cls, 4499999) == 17);
    REQUIRE(advTable.get_level(cls, 4500000) == 18);
    REQUIRE(advTable.get_level(cls, 4999999) == 18);
    REQUIRE(advTable.get_level(cls, 5000000) == 19);
    REQUIRE(advTable.get_level(cls, 5499999) == 19);
    REQUIRE(advTable.get_level(cls, 5500000) == 20);
    REQUIRE(advTable.get_level(cls, 5999999) == 20);
    REQUIRE(advTable.get_level(cls, 6000000) == 21);
}

TEST_CASE("[TC-ADVT.010] Level lookup for thief/bard yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::thief,
        Defs::character_class::bard
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1200) == 1);
        REQUIRE(advTable.get_level(cls, 1250) == 2);
        REQUIRE(advTable.get_level(cls, 4999) == 3);
        REQUIRE(advTable.get_level(cls, 5000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 10);
        REQUIRE(advTable.get_level(cls, 500001) == 12);
        REQUIRE(advTable.get_level(cls, 1000100) == 14);
        REQUIRE(advTable.get_level(cls, 1500000) == 16);
        REQUIRE(advTable.get_level(cls, 1550000) == 17);
        REQUIRE(advTable.get_level(cls, 2000000) == 19);
        REQUIRE(advTable.get_level(cls, 2200000) == 20);
        REQUIRE(advTable.get_level(cls, 2400000) == 20);
        REQUIRE(advTable.get_level(cls, 2420000) == 21);
    }
}

TEST_CASE("[TC-EXPE.001] Experience gain/loss changes level accordingly for paladin", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::paladin;

    Experience exp{cls};
    exp += 1000;
    REQUIRE(exp.xp() == 1000);
    REQUIRE(exp.level() == 1);

    exp += 1000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 1);

    exp += 250;
    REQUIRE(exp.xp() == 2250);
    REQUIRE(exp.level() == 2);

    exp += 2250;
    REQUIRE(exp.xp() == 4500);
    REQUIRE(exp.level() == 3);

    exp += 4499;
    REQUIRE(exp.xp() == 8999);
    REQUIRE(exp.level() == 3);

    exp += 1;
    REQUIRE(exp.xp() == 9000);
    REQUIRE(exp.level() == 4);

    exp -= 1;
    REQUIRE(exp.xp() == 8999);
    REQUIRE(exp.level() == 3);

    exp += 1;
    REQUIRE(exp.xp() == 9000);
    REQUIRE(exp.level() == 4);

    exp -= 6000;
    REQUIRE(exp.xp() == 3000);
    REQUIRE(exp.level() == 2);
}

TEST_CASE("[TC-EXPE.002] Experience gain/loss changes level accordingly for ranger", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::ranger;

    Experience exp{cls};
    exp += 1000;
    REQUIRE(exp.xp() == 1000);
    REQUIRE(exp.level() == 1);

    exp += 1000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 1);

    exp += 250;
    REQUIRE(exp.xp() == 2250);
    REQUIRE(exp.level() == 2);

    exp += 2250;
    REQUIRE(exp.xp() == 4500);
    REQUIRE(exp.level() == 3);

    exp += 4499;
    REQUIRE(exp.xp() == 8999);
    REQUIRE(exp.level() == 3);

    exp += 1;
    REQUIRE(exp.xp() == 9000);
    REQUIRE(exp.level() == 4);

    exp -= 1;
    REQUIRE(exp.xp() == 8999);
    REQUIRE(exp.level() == 3);

    exp += 1;
    REQUIRE(exp.xp() == 9000);
    REQUIRE(exp.level() == 4);

    exp -= 6000;
    REQUIRE(exp.xp() == 3000);
    REQUIRE(exp.level() == 2);

    exp += 2000000;
    REQUIRE(exp.xp() == 2003000);
    REQUIRE(exp.level() == 14);
}

TEST_CASE("[TC-EXPE.003] Experience gain/loss changes level accordingly for fighter", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::fighter;

    Experience exp{cls};
    exp += 1000;
    REQUIRE(exp.xp() == 1000);
    REQUIRE(exp.level() == 1);

    exp += 1000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 2);

    exp += 250;
    REQUIRE(exp.xp() == 2250);
    REQUIRE(exp.level() == 2);

    exp += 2250;
    REQUIRE(exp.xp() == 4500);
    REQUIRE(exp.level() == 3);

    exp += 4400;
    REQUIRE(exp.xp() == 8900);
    REQUIRE(exp.level() == 4);

    exp += 100;
    REQUIRE(exp.xp() == 9000);
    REQUIRE(exp.level() == 4);

    exp += 250000;
    REQUIRE(exp.xp() == 259000);
    REQUIRE(exp.level() == 9);

    exp -= 200000;
    REQUIRE(exp.xp() == 59000);
    REQUIRE(exp.level() == 6);
}

TEST_CASE("[TC-EXPE.004] Experience gain/loss changes level accordingly for mage", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::mage;

    Experience exp{cls};
    exp += 2000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 1);

    exp += 500;
    REQUIRE(exp.xp() == 2500);
    REQUIRE(exp.level() == 2);

    exp += 5000;
    REQUIRE(exp.xp() == 7500);
    REQUIRE(exp.level() == 3);

    exp += 60000;
    REQUIRE(exp.xp() == 67500);
    REQUIRE(exp.level() == 7);

    exp += 1500000;
    REQUIRE(exp.xp() == 1567500);
    REQUIRE(exp.level() == 14);

    exp -= 1000000;
    REQUIRE(exp.xp() == 567500);
    REQUIRE(exp.level() == 11);
}

TEST_CASE("[TC-EXPE.005] Experience gain/loss changes level accordingly for cleric", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::cleric;

    Experience exp{cls};
    exp += 2000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 2);

    exp += 500;
    REQUIRE(exp.xp() == 2500);
    REQUIRE(exp.level() == 2);

    exp += 5000;
    REQUIRE(exp.xp() == 7500);
    REQUIRE(exp.level() == 4);

    exp += 60000;
    REQUIRE(exp.xp() == 67500);
    REQUIRE(exp.level() == 7);

    exp += 1500000;
    REQUIRE(exp.xp() == 1567500);
    REQUIRE(exp.level() == 14);

    exp -= 1000000;
    REQUIRE(exp.xp() == 567500);
    REQUIRE(exp.level() == 10);
}

TEST_CASE("[TC-EXPE.006] Experience gain/loss changes level accordingly for druid", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::druid;

    Experience exp{cls};
    exp += 2000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 2);

    exp += 500;
    REQUIRE(exp.xp() == 2500);
    REQUIRE(exp.level() == 2);

    exp += 5000;
    REQUIRE(exp.xp() == 7500);
    REQUIRE(exp.level() == 4);

    exp += 60000;
    REQUIRE(exp.xp() == 67500);
    REQUIRE(exp.level() == 8);

    exp += 1500000;
    REQUIRE(exp.xp() == 1567500);
    REQUIRE(exp.level() == 14);

    exp -= 1000000;
    REQUIRE(exp.xp() == 567500);
    REQUIRE(exp.level() == 12);
}

TEST_CASE("[TC-EXPE.007] Experience gain/loss changes level accordingly for thief", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::thief;

    Experience exp{cls};
    exp += 2000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 2);

    exp += 500;
    REQUIRE(exp.xp() == 2500);
    REQUIRE(exp.level() == 3);

    exp += 5000;
    REQUIRE(exp.xp() == 7500);
    REQUIRE(exp.level() == 4);

    exp += 60000;
    REQUIRE(exp.xp() == 67500);
    REQUIRE(exp.level() == 7);

    exp += 1500000;
    REQUIRE(exp.xp() == 1567500);
    REQUIRE(exp.level() == 17);

    exp -= 1000000;
    REQUIRE(exp.xp() == 567500);
    REQUIRE(exp.level() == 12);
}

TEST_CASE("[TC-EXPE.008] Experience gain/loss changes level accordingly for bard", "[experience]" )
{
    Defs::character_class cls = Defs::character_class::bard;

    Experience exp{cls};
    exp += 2000;
    REQUIRE(exp.xp() == 2000);
    REQUIRE(exp.level() == 2);

    exp += 500;
    REQUIRE(exp.xp() == 2500);
    REQUIRE(exp.level() == 3);

    exp += 5000;
    REQUIRE(exp.xp() == 7500);
    REQUIRE(exp.level() == 4);

    exp += 60000;
    REQUIRE(exp.xp() == 67500);
    REQUIRE(exp.level() == 7);

    exp += 1500000;
    REQUIRE(exp.xp() == 1567500);
    REQUIRE(exp.level() == 17);

    exp -= 1000000;
    REQUIRE(exp.xp() == 567500);
    REQUIRE(exp.level() == 12);
}

TEST_CASE("[TC-HITP.001] Increasing length of HP sequences generates new HD values", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsT);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();
}

TEST_CASE("[TC-HITP.002] Shrink of HP sequences reduces HP values", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};

    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsT);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.shrink(clsF);
    REQUIRE(hp.total() < previousTotal);
    previousTotal = hp.total();

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
}

TEST_CASE("[TC-HITP.003] Shrink and increase of HP sequence keep the previous HD generated value", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};

    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 4);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    previousTotal = hp.total();

    hp.shrink(clsF);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() < previousTotal);

    hp.increase(clsF);
    REQUIRE(hp.level(clsF) == 4);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() == previousTotal);
}