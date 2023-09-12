//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-CHAR.001] Character's skills are generated according to the class/race boundaries for multiclass", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage};
    Defs::race chrRace{Defs::race::elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};

    REQUIRE(chr.strength() >= 9);
    REQUIRE(chr.strength() < 19);
    REQUIRE(chr.dexterity() >= 7);
    REQUIRE(chr.dexterity() <= 19);
    REQUIRE(chr.constitution() >= 6);
    REQUIRE(chr.constitution() <= 17);
    REQUIRE(chr.intelligence() >= 9);
    REQUIRE(chr.intelligence() <= 18);
    REQUIRE(chr.wisdom() >= 3);
    REQUIRE(chr.wisdom() <= 18);
    REQUIRE(chr.charisma() >= 8);
    REQUIRE(chr.charisma() <= 18);
}

TEST_CASE("[TC-CHAR.002] Paladin's skills are generated according to the class boundaries", "[character]" )
{
    std::string chrName{"Piergeiron"};
    Defs::character_class chrClass{Defs::character_class::paladin};
    Defs::race chrRace{Defs::race::human};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};

    REQUIRE(chr.strength() >= 12);
    REQUIRE(chr.strength() < 19);
    REQUIRE(chr.constitution() >= 9);
    REQUIRE(chr.constitution() <= 18);
    REQUIRE(chr.wisdom() >= 13);
    REQUIRE(chr.wisdom() <= 18);
    REQUIRE(chr.charisma() >= 17);
    REQUIRE(chr.charisma() <= 18);
}

TEST_CASE("[TC-CHAR.003] Gained XPs are broken down for multiclass characters", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};

    chr.gain_xp(1900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 633);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 633);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 633);
}

TEST_CASE("[TC-CHAR.004] Multiclasses can gain XPs in a single class, no split happens", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));

    chr.gain_xp(650, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 0);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 0);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 650);
}

TEST_CASE("[TC-CHAR.005] Lost XPs are broken down for multiclasses", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1300);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 1200);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 1200);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1200);
}

TEST_CASE("[TC-CHAR.006] When levels are gained, HP are increment accordingly", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 13));
    
    REQUIRE(chr.hp() == 6);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1300);
    REQUIRE(chr.hp() == 8);

    chr.gain_xp(2100);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 2000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 2000);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 2000);
    REQUIRE(chr.hp() == 11);

    chr.gain_xp(1500);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 2500);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 2500);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 2500);
    REQUIRE(chr.hp() == 14);
}

TEST_CASE("[TC-CHAR.007] When levels are lost, HPs are restored to the previous state", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 13));

    REQUIRE(chr.hp() == 6);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 1300);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1300);
    REQUIRE(chr.hp() == 8);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 1200);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 1200);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1200);
    REQUIRE(chr.hp() == 6);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 10200);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 10200);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 10200);
    REQUIRE(chr.hp() == 26);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 9900);
    REQUIRE(chr.hp() == 23);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 6000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 9900);
    REQUIRE(chr.hp() == 20);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 6000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 1875);
    REQUIRE(chr.hp() == 16);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 6000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 625);
    REQUIRE(chr.hp() == 14);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 3000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 9900);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 625);
    REQUIRE(chr.hp() == 11);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == 3000);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::mage) == 3750);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::thief) == 625);
    REQUIRE(chr.hp() == 10);
}

TEST_CASE("[TC-CHAR.008] When levels are lost, HPs are restored to the previous state, constitution equals 15", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 15));

    REQUIRE(chr.hp() == 7);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 9);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 7);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.hp() == 27);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 24);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 21);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 17);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 15);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 12);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 11);
}

TEST_CASE("[TC-CHAR.009] When levels are lost, HPs are restored to the previous state, constitution equals 16", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 16));

    REQUIRE(chr.hp() == 8);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 10);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 8);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.hp() == 28);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 25);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 22);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 18);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 16);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 13);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 12);
}

TEST_CASE("[TC-CHAR.010] When levels are lost, HPs are restored to the previous state, constitution equals 17", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 17));

    REQUIRE(chr.hp() == 9);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 12);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 9);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.hp() == 39);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 34);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 30);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 24);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 21);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 17);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 15);
}

TEST_CASE("[TC-CHAR.011] When levels are lost, HPs are restored to the previous state, constitution equals 18", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 18));

    REQUIRE(chr.hp() == 10);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 13);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 10);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.hp() == 40);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 35);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 31);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 25);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 22);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 18);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 16);
}

TEST_CASE("[TC-CHAR.012] When levels are lost, HPs are restored to the previous state, constitution equals 19", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));
    chr.change_skill(SkillValue(Defs::skill::intelligence, 12));
    chr.change_skill(SkillValue(Defs::skill::dexterity, 12));
    chr.change_skill(SkillValue(Defs::skill::constitution, 19));
    
    REQUIRE(chr.hp() == 11);

    chr.gain_xp(3900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 14);

    chr.lose_xp(300);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 1);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 11);

    chr.gain_xp(27000);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 5);
    REQUIRE(chr.hp() == 41);

    chr.lose_xp(900);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 4);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 36);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 4);
    REQUIRE(chr.hp() == 32);

    chr.lose_level(2, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 2);
    REQUIRE(chr.hp() == 26);

    chr.lose_level(1, Defs::character_class::thief);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 23);

    chr.lose_level(1, Defs::character_class::fighter);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 3);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 19);

    chr.lose_level(1, Defs::character_class::mage);
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::mage) == 2);
    REQUIRE(chr.experience().level(Defs::character_class::thief) == 1);
    REQUIRE(chr.hp() == 17);
}

TEST_CASE("[TC-CHAR.013] When levels exceed the title level, a fixed amount of HPs is granted for each HD beyond", "[character]" )
{
    std::string chrName{"Palantir"};
    Defs::character_class chrClass{Defs::character_class::paladin};
    Defs::race chrRace{Defs::race::human};
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::constitution, 13));

    chr.gain_level(11);
    REQUIRE(chr.experience().level(Defs::character_class::paladin) == 12);
    REQUIRE(chr.hp() == 99);

    chr.change_skill(SkillValue(Defs::skill::constitution, 15));
    REQUIRE(chr.hp() == 108);

    chr.change_skill(SkillValue(Defs::skill::constitution, 16));
    REQUIRE(chr.hp() == 117);

    chr.change_skill(SkillValue(Defs::skill::constitution, 17));
    REQUIRE(chr.hp() == 126);

    chr.change_skill(SkillValue(Defs::skill::constitution, 18));
    REQUIRE(chr.hp() == 135);

    chr.change_skill(SkillValue(Defs::skill::constitution, 19));
    REQUIRE(chr.hp() == 144);
}

TEST_CASE("[TC-CHAR.014] Single-class characters with high prime requisites have 10% bonus on XPs", "[character]" )
{
    std::string chrName{"Palantir"};
    Defs::character_class chrClass{Defs::character_class::paladin};
    Defs::race chrRace{Defs::race::human};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 17));

    XP pts = 2200;
    XP adjPts = pts + pts * Const::xp_bonus_10;
    chr.gain_xp(pts);
    REQUIRE(chr.experience().level(Defs::character_class::paladin) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::paladin) == adjPts);
}

TEST_CASE("[TC-CHAR.014] Multi-class characters with high prime requisites have 10% bonus on XPs for class with high requisite", "[character]" )
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_cleric};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 15));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 18));

    XP pts = 3900;
    chr.gain_xp(pts);

    XP ptsFract = pts / 2;
    XP adjPtsFighter = ptsFract + ptsFract * Const::xp_bonus_none;
    XP adjPtsCleric = ptsFract + ptsFract * Const::xp_bonus_10;
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == adjPtsFighter);
    REQUIRE(chr.experience().level(Defs::character_class::cleric) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::cleric) == adjPtsCleric);
}

TEST_CASE("[TC-CHAR.015] Multi-class characters with high prime requisites have 10% bonus on XPs for class with high requisite", "[character]" )
{
    std::string chrName{"Jaheira"};
    Defs::character_class chrClass{Defs::character_class::fighter_druid};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 15));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 17));
    chr.change_skill(SkillValue(Defs::skill::charisma, 16));

    XP pts = 3900;
    chr.gain_xp(pts);

    XP ptsFract = pts / 2;
    XP adjPtsFighter = ptsFract + ptsFract * Const::xp_bonus_none;
    XP adjPtsDriud = ptsFract + ptsFract * Const::xp_bonus_10;
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == adjPtsFighter);
    REQUIRE(chr.experience().level(Defs::character_class::druid) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::druid) == adjPtsDriud);
}

TEST_CASE("[TC-CHAR.016] Multi-class characters with high prime requisites have 10% bonus on XPs for class with high requisite", "[character]" )
{
    std::string chrName{"Jaheira"};
    Defs::character_class chrClass{Defs::character_class::fighter_druid};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 15));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 17));
    chr.change_skill(SkillValue(Defs::skill::charisma, 15));

    XP pts = 3900;
    chr.gain_xp(pts);

    XP ptsFract = pts / 2;
    XP adjPtsFighter = ptsFract + ptsFract * Const::xp_bonus_none;
    XP adjPtsDriud = ptsFract + ptsFract * Const::xp_bonus_none;
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == adjPtsFighter);
    REQUIRE(chr.experience().level(Defs::character_class::druid) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::druid) == adjPtsDriud);
}

TEST_CASE("[TC-CHAR.017] Multi-class characters with high prime requisites have 10% bonus on XPs for class with high requisite", "[character]" )
{
    std::string chrName{"Jaheira"};
    Defs::character_class chrClass{Defs::character_class::fighter_druid};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 17));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 15));
    chr.change_skill(SkillValue(Defs::skill::charisma, 17));

    XP pts = 3900;
    chr.gain_xp(pts);

    XP ptsFract = pts / 2;
    XP adjPtsFighter = ptsFract + ptsFract * Const::xp_bonus_10;
    XP adjPtsDriud = ptsFract + ptsFract * Const::xp_bonus_none;
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == adjPtsFighter);
    REQUIRE(chr.experience().level(Defs::character_class::druid) == 1);
    REQUIRE(chr.experience().xp(Defs::character_class::druid) == adjPtsDriud);
}

TEST_CASE("[TC-CHAR.018] Multi-class characters with high prime requisites have 10% bonus on XPs for class with high requisite", "[character]" )
{
    std::string chrName{"Jaheira"};
    Defs::character_class chrClass{Defs::character_class::fighter_druid};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::strength, 18, 34));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 16));
    chr.change_skill(SkillValue(Defs::skill::charisma, 17));

    XP pts = 3900;
    chr.gain_xp(pts);

    XP ptsFract = pts / 2;
    XP adjPtsFighter = ptsFract + ptsFract * Const::xp_bonus_10;
    XP adjPtsDriud = ptsFract + ptsFract * Const::xp_bonus_10;
    REQUIRE(chr.experience().level(Defs::character_class::fighter) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::fighter) == adjPtsFighter);
    REQUIRE(chr.experience().level(Defs::character_class::druid) == 2);
    REQUIRE(chr.experience().xp(Defs::character_class::druid) == adjPtsDriud);
}

TEST_CASE("[TC-CHAR.019] Experience advencements are reflected on the spells availability", "[character]" )
{
    std::string chrName{"Salazar"};
    Defs::character_class chrClass{Defs::character_class::mage_cleric};
    Defs::race chrRace{Defs::race::half_elf};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male};
    chr.change_skill(SkillValue(Defs::skill::intelligence, 13));
    chr.change_skill(SkillValue(Defs::skill::wisdom, 12));

    XP pts = 50000;
    chr.gain_xp(pts);
    
    REQUIRE(chr.spell_book().total_slots(1) == 4);
    REQUIRE(chr.spell_book().total_slots(2) == 2);
    REQUIRE(chr.spell_book().total_slots(3) == 1);

    REQUIRE(chr.holy_symbol().total_slots(1) == 3);
    REQUIRE(chr.holy_symbol().total_slots(2) == 3);
    REQUIRE(chr.holy_symbol().total_slots(3) == 1);
}

TEST_CASE("[TC-CHAR.020] Non-magic users do not have spell book", "[character]" )
{
    std::string chrName{"Jack the giant-killer"};
    Defs::character_class chrClass{Defs::character_class::ranger};
    Defs::race chrRace{Defs::race::human};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male}; 
    REQUIRE_THROWS_AS(chr.spell_book(), std::runtime_error);
}

TEST_CASE("[TC-CHAR.021] Non-magic users do not have holy symbol", "[character]" )
{
    std::string chrName{"Etienne Javert de Montford"};
    Defs::character_class chrClass{Defs::character_class::thief};
    Defs::race chrRace{Defs::race::human};

    Character chr{chrName, chrClass, chrRace, Defs::sex::male}; 
    REQUIRE_THROWS_AS(chr.holy_symbol(), std::runtime_error);
}

TEST_CASE("[TC-CHAR.022] Characters can be queried for their ability to cast spells", "[character]" )
{
    Defs::race chrRace{Defs::race::human};

    REQUIRE_FALSE(Character("F", Defs::character_class::fighter, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("P", Defs::character_class::paladin, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("R", Defs::character_class::ranger, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("M", Defs::character_class::mage, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("M", Defs::character_class::abjurer, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MA", Defs::character_class::conjurer, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MD", Defs::character_class::diviner, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("ME", Defs::character_class::enchanter, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MI", Defs::character_class::illusionist, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MK", Defs::character_class::invoker, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MN", Defs::character_class::necromancer, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MT", Defs::character_class::transmuter, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("C", Defs::character_class::cleric, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("D", Defs::character_class::druid, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("PSM", Defs::character_class::preist_of_specific_mythos, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE_FALSE(Character("T", Defs::character_class::thief, chrRace, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("B", Defs::character_class::bard, chrRace, Defs::sex::male).is_spell_caster());

    REQUIRE(Character("FM", Defs::character_class::fighter_mage, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FI", Defs::character_class::fighter_illusionist, Defs::race::gnome, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FC", Defs::character_class::fighter_cleric, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("CR", Defs::character_class::cleric_ranger, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MC", Defs::character_class::mage_cleric, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FMC", Defs::character_class::fighter_mage_cleric, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("CI", Defs::character_class::cleric_illusionist, Defs::race::gnome, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FD", Defs::character_class::fighter_druid, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MD", Defs::character_class::mage_druid, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FMD", Defs::character_class::fighter_mage_druid, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE_FALSE(Character("FT", Defs::character_class::fighter_thief, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MT", Defs::character_class::mage_thief, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FMT", Defs::character_class::fighter_mage_thief, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("IT", Defs::character_class::illusionist_thief, Defs::race::gnome, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("CT", Defs::character_class::cleric_thief, Defs::race::half_elf, Defs::sex::male).is_spell_caster());
}

TEST_CASE("[TC-CHAR.023] Only magic user characters can use the spell book", "[character]" )
{
    Defs::race chrRace{Defs::race::human};

    Character elfRanger{"ER", Defs::character_class::ranger, Defs::race::elf, Defs::sex::male};
    Character dwarfCleric{"DC", Defs::character_class::cleric, Defs::race::dwarf, Defs::sex::male};
    Character halflingThief{"HT", Defs::character_class::thief, Defs::race::halfling, Defs::sex::male};
    Character gnomeFighter{"GF", Defs::character_class::fighter, Defs::race::gnome, Defs::sex::male};
    Character humanPaladin{"HP", Defs::character_class::paladin, Defs::race::human, Defs::sex::male};
    Character halfElfFighterCleric{"HEFC", Defs::character_class::fighter_cleric, Defs::race::half_elf, Defs::sex::male};
    Character halfElfDruid{"HED", Defs::character_class::druid, Defs::race::half_elf, Defs::sex::male};

    std::vector<Character> chars { elfRanger, dwarfCleric, halflingThief, gnomeFighter, humanPaladin,
                halfElfFighterCleric, halfElfDruid };
    
    auto spellId = Defs::wizard_spell::magic_missile;
    for (auto& chr : chars)
    {
        REQUIRE_THROWS_AS(chr.learn_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.memorise_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.remove_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.erase_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.cast_spell(spellId), std::runtime_error);
    }
}

TEST_CASE("[TC-CHAR.024] Only paladins, rangers, clerics and druids can use the holy symbol", "[character]" )
{
    Defs::race chrRace{Defs::race::human};

    Character elfMage{"EM", Defs::character_class::mage, Defs::race::elf, Defs::sex::male};
    Character dwarfFighterThief{"DFT", Defs::character_class::fighter_thief, Defs::race::dwarf, Defs::sex::male};
    Character halflingThief{"HT", Defs::character_class::thief, Defs::race::halfling, Defs::sex::male};
    Character gnomeIllusionist{"GI", Defs::character_class::fighter, Defs::race::gnome, Defs::sex::male};
    Character humanNecromencer{"HN", Defs::character_class::necromancer, Defs::race::human, Defs::sex::male};
    Character halfElfBard{"HEB", Defs::character_class::bard, Defs::race::half_elf, Defs::sex::male};

    std::vector<Character> chars { elfMage, dwarfFighterThief, halflingThief, gnomeIllusionist, humanNecromencer,
                    halfElfBard };
    
    auto spellId = Defs::priest_spell::cure_light_wounds;
    for (auto& chr : chars)
    {
        REQUIRE_THROWS_AS(chr.memorise_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.remove_spell(spellId), std::runtime_error);
        REQUIRE_THROWS_AS(chr.cast_spell(spellId), std::runtime_error);
    }
}

TEST_CASE("[TC-CHAR.025] Characters' saving throws improve with experience", "[character]" )
{
    Defs::saving_throw savId = Defs::saving_throw::petrification;

    Character anya{"Anya", Defs::character_class::fighter, Defs::race::human, Defs::sex::female};
    REQUIRE(anya.save_score(savId) == 15);
    REQUIRE(SavingThrows::get_instance().save_as(anya.get_class(), anya.experience(), savId).first == Defs::character_class_type::warrior);
    REQUIRE(SavingThrows::get_instance().save_as(anya.get_class(), anya.experience(), savId).second == 1);

    anya.gain_level(7);
    REQUIRE(anya.save_score(savId) == 11);
    REQUIRE(SavingThrows::get_instance().save_as(anya.get_class(), anya.experience(), savId).first == Defs::character_class_type::warrior);
    REQUIRE(SavingThrows::get_instance().save_as(anya.get_class(), anya.experience(), savId).second == 8);
}

TEST_CASE("[TC-CHAR.026] Multiclass characters choose between the most favorable saving throw", "[character]" )
{
    Defs::saving_throw savId = Defs::saving_throw::petrification;

    Character sanRaal{"San Raal", Defs::character_class::fighter_thief, Defs::race::elf, Defs::sex::male};

    REQUIRE(sanRaal.save_score(savId) == 12);

    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).first == Defs::character_class_type::rogue);
    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).second == 1);

    sanRaal.gain_xp(110000, Defs::character_class::thief);
    REQUIRE(sanRaal.experience().level(Defs::character_class::fighter) == 1);
    REQUIRE(sanRaal.experience().level(Defs::character_class::thief) == 9);

    REQUIRE(sanRaal.save_score(savId) == 10);
    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).first == Defs::character_class_type::rogue);
    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).second == 9);

    sanRaal.gain_xp(250000, Defs::character_class::fighter);
    REQUIRE(sanRaal.experience().level(Defs::character_class::fighter) == 9);
    REQUIRE(sanRaal.experience().level(Defs::character_class::thief) == 9);

    REQUIRE(sanRaal.save_score(savId) == 9);
    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).first == Defs::character_class_type::warrior);
    REQUIRE(SavingThrows::get_instance().save_as(sanRaal.get_class(), sanRaal.experience(), savId).second == 9);
}