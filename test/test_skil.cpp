//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <skills.h>
#include <skill_creator.h>

using namespace Adndtk;

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

TEST_CASE("[TC-SKIL.002] Comparison between different skills do not throws exceptions", "[skills]" )
{
    SkillValue dex{Defs::skill::dexterity, 13};
    SkillValue con{Defs::skill::constitution, 13};

    REQUIRE_FALSE(dex == con);
    REQUIRE(dex != con);
    REQUIRE_FALSE(dex < con);
    REQUIRE_FALSE(dex <= con);
    REQUIRE_FALSE(dex >= con);
    REQUIRE_FALSE(dex > con);
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

TEST_CASE("[TC-SKIL.079] Comparison between skills greater than 20 works as expected", "[skills]" )
{
    {
        SkillValue sk1{Defs::skill::strength, 23};
        SkillValue sk2{Defs::skill::strength, 25};

        REQUIRE_FALSE(sk1 == sk2);
        REQUIRE(sk1 != sk2);
        REQUIRE(sk1 < sk2);
        REQUIRE(sk1 <= sk2);
        REQUIRE_FALSE(sk1 >= sk2);
        REQUIRE_FALSE(sk1 > sk2); 
    }
    {
        SkillValue sk1{Defs::skill::strength, 18, 100};
        SkillValue sk2{Defs::skill::strength, 19};

        REQUIRE_FALSE(sk1 == sk2);
        REQUIRE(sk1 != sk2);
        REQUIRE(sk1 < sk2);
        REQUIRE(sk1 <= sk2);
        REQUIRE_FALSE(sk1 >= sk2);
        REQUIRE_FALSE(sk1 > sk2);
    }
}