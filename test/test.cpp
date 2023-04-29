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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

    Experience exp{cls, Defs::race::human};
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

TEST_CASE("[TC-EXPE.009] Experience gain for demihumans (halfling) cannot exceed the limits", "[experience]" )
{
    std::map<Defs::character_class, ExperienceLevel> limits
    {
        {Defs::character_class::fighter, 9},
        {Defs::character_class::cleric, 8},
        {Defs::character_class::thief, 15}
    };

    XP xp = 2000000;
    for (auto& l : limits)
    {
        Experience exp{l.first, Defs::race::halfling};
        exp += xp;
        REQUIRE(exp.xp() == xp);
        REQUIRE(exp.level() == l.second);
    }
}

TEST_CASE("[TC-EXPE.010] Experience gain for demihumans (elf) cannot exceed the limits", "[experience]" )
{
    std::map<Defs::character_class, ExperienceLevel> limits
    {
        {Defs::character_class::fighter, 12},
        {Defs::character_class::ranger, 15},
        {Defs::character_class::mage, 15},
        {Defs::character_class::diviner, 15},
        {Defs::character_class::enchanter, 15},
        {Defs::character_class::cleric, 12},
        {Defs::character_class::thief, 12}
    };

    XP xp = 3000000;
    for (auto& l : limits)
    {
        Experience exp{l.first, Defs::race::elf};
        exp += xp;
        REQUIRE(exp.xp() == xp);
        REQUIRE(exp.level() == l.second);
    }
}

TEST_CASE("[TC-EXPE.010] Experience gain for demihumans (half_elf) cannot exceed the limits", "[experience]" )
{
    std::map<Defs::character_class, ExperienceLevel> limits
    {
        {Defs::character_class::fighter, 14},
        {Defs::character_class::ranger, 16},
        {Defs::character_class::mage, 12},
        {Defs::character_class::conjurer, 12},
        {Defs::character_class::diviner, 12},
        {Defs::character_class::enchanter, 12},
        {Defs::character_class::transmuter, 12},
        {Defs::character_class::cleric, 14},
        {Defs::character_class::druid, 9},
        {Defs::character_class::thief, 12}
    };

    XP xp = 3000000;
    for (auto& l : limits)
    {
        Experience exp{l.first, Defs::race::half_elf};
        exp += xp;
        REQUIRE(exp.xp() == xp);
        REQUIRE(exp.level() == l.second);
    }
}

TEST_CASE("[TC-EXPE.011] Experience gain for demihumans (dwarf) cannot exceed the limits", "[experience]" )
{
    std::map<Defs::character_class, ExperienceLevel> limits
    {
        {Defs::character_class::fighter, 15},
        {Defs::character_class::cleric, 10},
        {Defs::character_class::thief, 12}
    };

    XP xp = 3000000;
    for (auto& l : limits)
    {
        Experience exp{l.first, Defs::race::dwarf};
        exp += xp;
        REQUIRE(exp.xp() == xp);
        REQUIRE(exp.level() == l.second);
    }
}

TEST_CASE("[TC-EXPE.012] Experience gain for demihumans (gnome) cannot exceed the limits", "[experience]" )
{
    std::map<Defs::character_class, ExperienceLevel> limits
    {
        {Defs::character_class::fighter, 11},
        {Defs::character_class::illusionist, 15},
        {Defs::character_class::cleric, 9},
        {Defs::character_class::thief, 13}
    };

    XP xp = 3000000;
    for (auto& l : limits)
    {
        Experience exp{l.first, Defs::race::gnome};
        exp += xp;
        REQUIRE(exp.xp() == xp);
        REQUIRE(exp.level() == l.second);
    }
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

TEST_CASE("[TC-HITP.004] Shrink and increase of HP sequence change the current HP score", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HitPoints hp{cls};
    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsF);

    REQUIRE(hp.total() == hp.current());

    hp.shrink(clsF);
    REQUIRE(hp.total() == hp.current());

    HP wounds = 1;
    hp -= wounds;
    REQUIRE(hp.total() - wounds == hp.current());
    hp.shrink(clsF);
    REQUIRE(hp.total() - wounds == hp.current());
    hp.increase(clsF);
    REQUIRE(hp.total() - wounds == hp.current());
}

TEST_CASE("[TC-HITP.005] Dice roll yield at leas 1 HP for multiclass", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsM = Defs::character_class::mage;

    HitPoints hp{cls};

    HP prevHP = hp.total();
    hp.increase(clsM);
    REQUIRE(hp.total() > prevHP);
    
    prevHP = hp.total();
    hp.increase(clsM);
    REQUIRE(hp.total() > prevHP);
}

TEST_CASE("[TC-HITP.006] Level zero characters cannot advance", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HitPoints hp{cls};

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);

    hp.shrink(clsM);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 1);

    REQUIRE_NOTHROW(hp.increase(clsT));

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);

    REQUIRE_NOTHROW(hp.increase(clsF));

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);

    REQUIRE_THROWS(hp.increase(clsM));

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);
}

TEST_CASE("[TC-HITP.007] Level adjustments receive callbacks", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};

    HPChangeType chgType{};
    HP prevHP{};
    HP nextHP{};
    OnHPChange cbk = [&] (const HPChangeType& ct, const HP& prev, const HP& next) -> void
    {
        chgType = ct;
        prevHP = prev;
        nextHP = next;
    };
    hp += cbk;

    HP currentHP = hp.current();
    HP hpChange{};
    
    hpChange = 1;
    hp -= hpChange;
    REQUIRE(chgType == HPChangeType::wounded);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP - hpChange);
    currentHP = nextHP;
    
    hpChange = 1;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::healed);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP + hpChange);
    currentHP = nextHP;
    
    hpChange = 0;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::none);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP);
    currentHP = nextHP;
    
    hpChange = 10;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::none);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP);
    currentHP = nextHP;
    
    hpChange = 100;
    hp -= hpChange;
    REQUIRE(chgType == HPChangeType::dead);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == 0);
}

TEST_CASE("[TC-HITP.008] Max score for HD ensures maximum HP score", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};

    REQUIRE(hp.total() == 10);
    hp.increase(cls, 3);

    REQUIRE(hp.total() == 40);
    REQUIRE(hp.current() == 40);
    REQUIRE(hp.level(cls) == 4);
}

TEST_CASE("[TC-HITP.009] Level for multiclass is the highest level among the classes", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HitPoints hp{cls};

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);

    hp.increase(clsT, 3);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 4);

    REQUIRE(hp.level() == 4);

    hp.increase(clsF, 2);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 4);

    REQUIRE(hp.level() == 4);
}

TEST_CASE("[TC-SAVT.001] Saving throws for warrior change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::warrior;
    ExperienceLevel lvl{};

    lvl = 0;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 20);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 18);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 19);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 18);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 18);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 16);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 16);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 15);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 15);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
}

TEST_CASE("[TC-SAVT.002] Saving throws for wizard change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::wizard;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 3);
}

TEST_CASE("[TC-SAVT.003] Saving throws for priest change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::priest;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
}

TEST_CASE("[TC-SAVT.004] Saving throws for rogue change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::rogue;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
}

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

TEST_CASE("[TC-TUND.001] Turning undead (skeleton/1HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::skeleton, Defs::turnable::hd1})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.002] Turning undead (zombie) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::zombie})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.003] Turning undead (ghoul/2HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghoul, Defs::turnable::hd2})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.004] Turning undead (shadow/3-4HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::shadow, Defs::turnable::hd3, Defs::turnable::hd4})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.005] Turning undead (wight/5HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::wight, Defs::turnable::hd5})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy_more);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.006] Turning undead (ghast) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghast})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.007] Turning undead (wraith/6HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::wraith, Defs::turnable::hd6})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::destroy);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.008] Turning undead (mummy/7HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::mummy, Defs::turnable::hd7})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.009] Turning undead (spectre/8HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::spectre, Defs::turnable::hd8})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::turn);
        REQUIRE(res.second.has_value() == false);
    }
}

TEST_CASE("[TC-TUND.010] Turning undead (vampire/9HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::vampire, Defs::turnable::hd9})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 4);
    }
}

TEST_CASE("[TC-TUND.011] Turning undead (ghost/10HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::ghost, Defs::turnable::hd10})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 7);
    }
}

TEST_CASE("[TC-TUND.012] Turning undead (ghost/10HD) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::lich, Defs::turnable::hd11_or_more})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 10);
    }
}

TEST_CASE("[TC-TUND.013] Turning undead (special) improves with priest's experience", "[turn_undead]" )
{
    std::pair<Defs::turn_effect, std::optional<short>> res{};
    for (auto& undeadType : {Defs::turnable::special})
    {
        res = TurnUndead::get_instance().get(1, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(2, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(3, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(4, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(5, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(6, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(7, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(8, undeadType);
        REQUIRE(res.first == Defs::turn_effect::no_effect);
        REQUIRE(res.second.has_value() == false);

        res = TurnUndead::get_instance().get(9, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 20);

        res = TurnUndead::get_instance().get(10, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(11, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 19);

        res = TurnUndead::get_instance().get(12, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(13, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 16);

        res = TurnUndead::get_instance().get(14, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(15, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);

        res = TurnUndead::get_instance().get(16, undeadType);
        REQUIRE(res.first == Defs::turn_effect::percentage);
        REQUIRE(res.second.has_value() == true);
        REQUIRE(res.second.value() == 13);
    }
}

TEST_CASE("[TC-CEXC.001] CoinExchange gives the correct conversion ratio from CP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::copper_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 1.0);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 0.1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 0.02);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.01);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.002);
}

TEST_CASE("[TC-CEXC.002] CoinExchange gives the correct conversion ratio from SP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::silver_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 0.2);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.02);
}

TEST_CASE("[TC-CEXC.003] CoinExchange gives the correct conversion ratio from EP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::electrum_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 50);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.1);
}

TEST_CASE("[TC-CEXC.004] CoinExchange gives the correct conversion ratio from GP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::gold_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 100);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 2);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.2);
}

TEST_CASE("[TC-CEXC.005] CoinExchange gives the correct conversion ratio from SP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::platinum_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 500);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 50);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 1);
}

TEST_CASE("[TC-COIN.001] Comparison between coins of the same type yield expected results", "[coin]" )
{
    for (auto& t : {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece})
    {
        Coin c1{t, 10};
        Coin c2{t, 12};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
}

TEST_CASE("[TC-COIN.002] Comparison between coins of different same type yield expected results", "[coin]" )
{
    {
        Coin c1{Defs::coin::copper_piece, 10};
        Coin c2{Defs::coin::gold_piece, 10};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 100};
        Coin c2{Defs::coin::gold_piece, 10};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 102};
        Coin c2{Defs::coin::gold_piece, 1};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE_FALSE(c1 < c2);
        REQUIRE_FALSE(c1 <= c2);
        REQUIRE(c1 >= c2);
        REQUIRE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 100};
        Coin c2{Defs::coin::gold_piece, 1};

        REQUIRE(c1 == c2);
        REQUIRE_FALSE(c1 != c2);
        REQUIRE_FALSE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
}

TEST_CASE("[TC-COIN.003] Comparison between coin and scalar yield expected results", "[coin]" )
{
    Coin c1{Defs::coin::gold_piece, 10};

    REQUIRE(c1 == 10);
    REQUIRE_FALSE(c1 == 11);
    REQUIRE(c1 != 11);
    REQUIRE_FALSE(c1 != 10);
    REQUIRE(c1 < 11);
    REQUIRE_FALSE(c1 < 10);
    REQUIRE_FALSE(c1 < 9);
    REQUIRE(c1 <= 11);
    REQUIRE(c1 <= 10);
    REQUIRE_FALSE(c1 <= 9);
    REQUIRE(c1 >= 9);
    REQUIRE(c1 >= 10);
    REQUIRE_FALSE(c1 >= 11);
    REQUIRE(c1 > 9);
    REQUIRE_FALSE(c1 > 10);
    REQUIRE_FALSE(c1 > 11);
}