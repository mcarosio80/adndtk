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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male};

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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr};

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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};

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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_neutral, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::chaotic_good, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_neutral, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_neutral, Defs::sex::male};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::true_neutral, Defs::sex::male, Defs::deity::oghma};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::selune};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::true_neutral, Defs::sex::male, Defs::deity::oghma};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::selune};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::ilmater};
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

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr}; 
    REQUIRE_THROWS_AS(chr.spell_book(), std::runtime_error);
}

TEST_CASE("[TC-CHAR.021] Non-magic users do not have holy symbol", "[character]" )
{
    std::string chrName{"Etienne Javert de Montford"};
    Defs::character_class chrClass{Defs::character_class::thief};
    Defs::race chrRace{Defs::race::human};

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::lawful_evil, Defs::sex::male}; 
    REQUIRE_THROWS_AS(chr.holy_symbol(), std::runtime_error);
}

TEST_CASE("[TC-CHAR.022] Characters can be queried for their ability to cast spells", "[character]" )
{
    Defs::race chrRace{Defs::race::human};
    Defs::moral_alignment align{Defs::moral_alignment::chaotic_good};

    REQUIRE_FALSE(Character("F", Defs::character_class::fighter, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("P", Defs::character_class::paladin, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr).is_spell_caster());
    REQUIRE(Character("R", Defs::character_class::ranger, chrRace, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("M", Defs::character_class::mage, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("M", Defs::character_class::abjurer, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MA", Defs::character_class::conjurer, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MD", Defs::character_class::diviner, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("ME", Defs::character_class::enchanter, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MI", Defs::character_class::illusionist, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MK", Defs::character_class::invoker, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MN", Defs::character_class::necromancer, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MT", Defs::character_class::transmuter, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("C", Defs::character_class::cleric, chrRace, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("D", Defs::character_class::druid, chrRace, Defs::moral_alignment::true_neutral, Defs::sex::male, Defs::deity::oghma).is_spell_caster());
    REQUIRE(Character("PSM", Defs::character_class::preist_of_specific_mythos, chrRace, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr).is_spell_caster());
    REQUIRE_FALSE(Character("T", Defs::character_class::thief, chrRace, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("B", Defs::character_class::bard, chrRace, align, Defs::sex::male).is_spell_caster());

    REQUIRE(Character("FM", Defs::character_class::fighter_mage, Defs::race::half_elf, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FI", Defs::character_class::fighter_illusionist, Defs::race::gnome, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FC", Defs::character_class::fighter_cleric, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("CR", Defs::character_class::cleric_ranger, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("MC", Defs::character_class::mage_cleric, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("FMC", Defs::character_class::fighter_mage_cleric, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("CI", Defs::character_class::cleric_illusionist, Defs::race::gnome, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
    REQUIRE(Character("FD", Defs::character_class::fighter_druid, Defs::race::half_elf, Defs::moral_alignment::neutral_evil, Defs::sex::male, Defs::deity::lolth).is_spell_caster());
    REQUIRE(Character("MD", Defs::character_class::mage_druid, Defs::race::half_elf, Defs::moral_alignment::neutral_evil, Defs::sex::male, Defs::deity::lolth).is_spell_caster());
    REQUIRE(Character("FMD", Defs::character_class::fighter_mage_druid, Defs::race::half_elf, Defs::moral_alignment::neutral_evil, Defs::sex::male, Defs::deity::lolth).is_spell_caster());
    REQUIRE_FALSE(Character("FT", Defs::character_class::fighter_thief, Defs::race::half_elf, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("MT", Defs::character_class::mage_thief, Defs::race::half_elf, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("FMT", Defs::character_class::fighter_mage_thief, Defs::race::half_elf, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("IT", Defs::character_class::illusionist_thief, Defs::race::gnome, align, Defs::sex::male).is_spell_caster());
    REQUIRE(Character("CT", Defs::character_class::cleric_thief, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::tempus).is_spell_caster());
}

TEST_CASE("[TC-CHAR.023] Only magic user characters can use the spell book", "[character]" )
{
    Defs::race chrRace{Defs::race::human};
    Defs::moral_alignment align{Defs::moral_alignment::neutral_good};

    Character elfRanger{"ER", Defs::character_class::ranger, Defs::race::elf, align, Defs::sex::male, Defs::deity::lathander};
    Character dwarfCleric{"DC", Defs::character_class::cleric, Defs::race::dwarf, align, Defs::sex::male, Defs::deity::lathander};
    Character halflingThief{"HT", Defs::character_class::thief, Defs::race::halfling, align, Defs::sex::male};
    Character gnomeFighter{"GF", Defs::character_class::fighter, Defs::race::gnome, align, Defs::sex::male};
    Character humanPaladin{"HP", Defs::character_class::paladin, Defs::race::human, Defs::moral_alignment::lawful_good, Defs::sex::male, Defs::deity::tyr};
    Character halfElfFighterCleric{"HEFC", Defs::character_class::fighter_cleric, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::lathander};
    Character halfElfDruid{"HED", Defs::character_class::druid, Defs::race::half_elf, align, Defs::sex::male, Defs::deity::helm};

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
    Defs::moral_alignment align{Defs::moral_alignment::chaotic_good};

    Character elfMage{"EM", Defs::character_class::mage, Defs::race::elf, align, Defs::sex::male};
    Character dwarfFighterThief{"DFT", Defs::character_class::fighter_thief, Defs::race::dwarf, align, Defs::sex::male};
    Character halflingThief{"HT", Defs::character_class::thief, Defs::race::halfling, align, Defs::sex::male};
    Character gnomeIllusionist{"GI", Defs::character_class::fighter, Defs::race::gnome, align, Defs::sex::male};
    Character humanNecromencer{"HN", Defs::character_class::necromancer, Defs::race::human, align, Defs::sex::male};
    Character halfElfBard{"HEB", Defs::character_class::bard, Defs::race::half_elf, align, Defs::sex::male};

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

    Character anya{"Anya", Defs::character_class::fighter, Defs::race::human, Defs::moral_alignment::chaotic_good, Defs::sex::female};
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

    Character sanRaal{"San Raal", Defs::character_class::fighter_thief, Defs::race::elf, Defs::moral_alignment::chaotic_neutral, Defs::sex::male};

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

TEST_CASE("[TC-CHAR.027] Fighters, mages and clerics have no limitation on moral alignments", "[character]" )
{
    auto classes = {Defs::character_class::fighter, Defs::character_class::mage, Defs::character_class::cleric};

    for (auto& c : classes)
    {
        auto aligns = Cyclopedia::get_instance().available_moral_alignments(c);
        REQUIRE(aligns.size() == 9);

        REQUIRE(aligns.find(Defs::moral_alignment::lawful_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::lawful_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::lawful_evil) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::true_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::neutral_evil) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_evil) != aligns.end());
    }
}

TEST_CASE("[TC-CHAR.028] Thieves cannot be lawful good", "[character]" )
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(Defs::character_class::thief);
    REQUIRE(aligns.size() == 8);

    REQUIRE(aligns.find(Defs::moral_alignment::lawful_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::lawful_evil) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::true_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_evil) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_evil) != aligns.end());
}

TEST_CASE("[TC-CHAR.029] Druids can only be neutral", "[character]" )
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(Defs::character_class::druid);
    REQUIRE(aligns.size() == 3);

    REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::true_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_evil) != aligns.end());
}

TEST_CASE("[TC-CHAR.030] Paladins can only be lawful good", "[character]" )
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(Defs::character_class::paladin);
    REQUIRE(aligns.size() == 1);

    REQUIRE(aligns.find(Defs::moral_alignment::lawful_good) != aligns.end());
}

TEST_CASE("[TC-CHAR.031] Rangers can only be good", "[character]" )
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(Defs::character_class::ranger);
    REQUIRE(aligns.size() == 3);

    REQUIRE(aligns.find(Defs::moral_alignment::lawful_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_good) != aligns.end());
}

TEST_CASE("[TC-CHAR.032] Specialistic wizards have no limitation on moral alignments", "[character]" )
{
    auto classes = {
        Defs::character_class::abjurer,
        Defs::character_class::conjurer,
        Defs::character_class::diviner,
        Defs::character_class::enchanter,
        Defs::character_class::illusionist,
        Defs::character_class::invoker,
        Defs::character_class::necromancer,
        Defs::character_class::transmuter
    };

    for (auto& c : classes)
    {
        auto aligns = Cyclopedia::get_instance().available_moral_alignments(c);
        REQUIRE(aligns.size() == 9);

        REQUIRE(aligns.find(Defs::moral_alignment::lawful_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::lawful_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::lawful_evil) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::true_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::neutral_evil) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_good) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_neutral) != aligns.end());
        REQUIRE(aligns.find(Defs::moral_alignment::chaotic_evil) != aligns.end());
    }
}

TEST_CASE("[TC-CHAR.033] Bards have no limitation on moral alignments", "[character]" )
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(Defs::character_class::bard);
    REQUIRE(aligns.size() == 9);

    REQUIRE(aligns.find(Defs::moral_alignment::lawful_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::lawful_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::lawful_evil) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::true_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::neutral_evil) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_good) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_neutral) != aligns.end());
    REQUIRE(aligns.find(Defs::moral_alignment::chaotic_evil) != aligns.end());
}

TEST_CASE("[TC-CHAR.034] Priests of specific mythos must align to the deity of choice", "[character]" )
{
    auto clsId = Defs::character_class::preist_of_specific_mythos;

    auto alignsLathander = Cyclopedia::get_instance().available_moral_alignments(clsId, Defs::deity::lathander);
    REQUIRE(alignsLathander.size() == 4);

    REQUIRE(alignsLathander.find(Defs::moral_alignment::lawful_good) != alignsLathander.end());
    REQUIRE(alignsLathander.find(Defs::moral_alignment::lawful_neutral) != alignsLathander.end());
    REQUIRE(alignsLathander.find(Defs::moral_alignment::neutral_good) != alignsLathander.end());
    REQUIRE(alignsLathander.find(Defs::moral_alignment::chaotic_good) != alignsLathander.end());

    auto alignsLolth = Cyclopedia::get_instance().available_moral_alignments(clsId, Defs::deity::lolth);
    REQUIRE(alignsLolth.size() == 4);

    REQUIRE(alignsLolth.find(Defs::moral_alignment::lawful_evil) != alignsLolth.end());
    REQUIRE(alignsLolth.find(Defs::moral_alignment::neutral_evil) != alignsLolth.end());
    REQUIRE(alignsLolth.find(Defs::moral_alignment::chaotic_neutral) != alignsLolth.end());
    REQUIRE(alignsLolth.find(Defs::moral_alignment::chaotic_evil) != alignsLolth.end());

    auto alignsTyr = Cyclopedia::get_instance().available_moral_alignments(clsId, Defs::deity::tyr);
    REQUIRE(alignsTyr.size() == 3);

    REQUIRE(alignsTyr.find(Defs::moral_alignment::lawful_good) != alignsTyr.end());
    REQUIRE(alignsTyr.find(Defs::moral_alignment::lawful_neutral) != alignsTyr.end());
    REQUIRE(alignsTyr.find(Defs::moral_alignment::neutral_good) != alignsTyr.end());
}

TEST_CASE("[TC-CHAR.035] Deities can be chosen according to the character's moral alignment", "[character]" )
{
    auto deitiesTN = Cyclopedia::get_instance().available_deities(Defs::moral_alignment::true_neutral);

    REQUIRE(deitiesTN.size() == 10);
    REQUIRE(deitiesTN.find(Defs::deity::helm) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::tempus) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::selune) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::chauntea) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::mystryl) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::moradin) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::oghma) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::sune) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::cyric) != deitiesTN.end());
    REQUIRE(deitiesTN.find(Defs::deity::corellon) != deitiesTN.end());

    auto deitiesCE = Cyclopedia::get_instance().available_deities(Defs::moral_alignment::chaotic_evil);

    REQUIRE(deitiesCE.size() == 7);
    REQUIRE(deitiesCE.find(Defs::deity::bane) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::tempus) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::lolth) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::mystryl) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::talos) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::cyric) != deitiesCE.end());
    REQUIRE(deitiesCE.find(Defs::deity::corellon) != deitiesCE.end());
}

TEST_CASE("[TC-CHAR.036] Deity selection is mandatory for priest spells casters", "[character]" )
{
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_good;
    auto sex = Defs::sex::male;

    REQUIRE_NOTHROW(Character("PSM", Defs::character_class::preist_of_specific_mythos, race, align, sex, Defs::deity::tyr));
    REQUIRE_THROWS_AS(Character("PSM", Defs::character_class::preist_of_specific_mythos, race, align, sex), std::runtime_error);
    
    REQUIRE_NOTHROW(Character("C", Defs::character_class::cleric, race, align, sex, Defs::deity::tyr));
    REQUIRE_THROWS_AS(Character("C", Defs::character_class::cleric, race, align, sex), std::runtime_error);
    
    REQUIRE_NOTHROW(Character("D", Defs::character_class::druid, race, Defs::moral_alignment::true_neutral, sex, Defs::deity::selune));
    REQUIRE_THROWS_AS(Character("D", Defs::character_class::druid, race, Defs::moral_alignment::true_neutral, sex), std::runtime_error);
    
    REQUIRE_NOTHROW(Character("P", Defs::character_class::paladin, race, align, sex, Defs::deity::tyr));
    REQUIRE_THROWS_AS(Character("P", Defs::character_class::paladin, race, align, sex), std::runtime_error);
    
    REQUIRE_NOTHROW(Character("R", Defs::character_class::ranger, race, align, sex, Defs::deity::tyr));
    REQUIRE_THROWS_AS(Character("R", Defs::character_class::ranger, race, align, sex), std::runtime_error);
}

TEST_CASE("[TC-CHAR.037] Deity selection for Priests of specific mythos must agree with the character's moral alignment", "[character]" )
{
    auto cls = Defs::character_class::preist_of_specific_mythos;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_good;
    auto sex = Defs::sex::male;

    REQUIRE_NOTHROW(Character("PSM", cls, race, align, sex, Defs::deity::tyr));
    REQUIRE_THROWS_AS(Character("PSM", cls, race, align, sex, Defs::deity::bane), std::runtime_error);
}

TEST_CASE("[TC-CHAR.038] Aging affect skill values", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};
    auto currentAge = chr.age();

    REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
    REQUIRE(chr.age() == currentAge);

    auto strVal = chr.skill(Defs::skill::strength);
    auto conVal = chr.skill(Defs::skill::constitution);
    auto intVal = chr.skill(Defs::skill::intelligence);
    auto wisVal = chr.skill(Defs::skill::wisdom);
    auto years = 45 - currentAge + 1;
    chr.grow_old(years);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::middle_age);
    REQUIRE(chr.age() == 46);

    REQUIRE(chr.skill(Defs::skill::strength) == strVal - 1);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal - 1);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal + 1);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal + 1);
}

TEST_CASE("[TC-CHAR.039] Rejuvenating restore previous skill values", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};

    chr.grow_old(40);
    auto strValMiddleAged = chr.skill(Defs::skill::strength);
    auto conValMiddleAged = chr.skill(Defs::skill::constitution);
    auto intValMiddleAged = chr.skill(Defs::skill::intelligence);
    auto wisValMiddleAged = chr.skill(Defs::skill::wisdom);

    chr.rejuvenate(40);
    REQUIRE(chr.skill(Defs::skill::strength) == strValMiddleAged + 1);
    REQUIRE(chr.skill(Defs::skill::constitution) == conValMiddleAged + 1);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intValMiddleAged - 1);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisValMiddleAged - 1);
}

TEST_CASE("[TC-CHAR.040] Growing very old in one shot effects cumulate", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};
    auto currentAge = chr.age();

    REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
    REQUIRE(chr.age() == currentAge);

    auto strVal = chr.skill(Defs::skill::strength);
    auto dexVal = chr.skill(Defs::skill::dexterity);
    auto conVal = chr.skill(Defs::skill::constitution);
    auto intVal = chr.skill(Defs::skill::intelligence);
    auto wisVal = chr.skill(Defs::skill::wisdom);
    auto years = 60 - currentAge + 1;
    chr.grow_old(years);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::old_age);
    REQUIRE(chr.age() == 61);

    REQUIRE(chr.skill(Defs::skill::strength) == strVal - 3);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal - 2);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal - 2);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal + 1);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal + 2);
}

TEST_CASE("[TC-CHAR.041] Becoming venerable in one shot effects cumulate", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};
    auto currentAge = chr.age();

    REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
    REQUIRE(chr.age() == currentAge);

    auto strVal = chr.skill(Defs::skill::strength);
    auto dexVal = chr.skill(Defs::skill::dexterity);
    auto conVal = chr.skill(Defs::skill::constitution);
    auto intVal = chr.skill(Defs::skill::intelligence);
    auto wisVal = chr.skill(Defs::skill::wisdom);
    auto years = 90 - currentAge + 1;
    chr.grow_old(years);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::venerable);
    REQUIRE(chr.age() == 91);

    REQUIRE(chr.skill(Defs::skill::strength) == strVal - 4);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal - 3);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal - 3);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal + 2);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal + 3);
}

TEST_CASE("[TC-CHAR.042] Rejuvenate from venerable in one shot regains the skills", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};

    chr.grow_old(75);
    auto strVal = chr.skill(Defs::skill::strength);
    auto dexVal = chr.skill(Defs::skill::dexterity);
    auto conVal = chr.skill(Defs::skill::constitution);
    auto intVal = chr.skill(Defs::skill::intelligence);
    auto wisVal = chr.skill(Defs::skill::wisdom);

    chr.rejuvenate(75);
    REQUIRE(chr.skill(Defs::skill::strength) == strVal + 4);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal + 3);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal + 3);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal - 2);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal - 3);
}

TEST_CASE("[TC-CHAR.043] Rejuvenate from venerable in steps regains the skills gradually", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};

    auto currentAge = chr.age();
    auto years = 90 - currentAge + 1;
    chr.grow_old(years);
    
    REQUIRE(chr.get_age_range() == Defs::aging_effects::venerable);
    auto strVal = chr.skill(Defs::skill::strength);
    auto dexVal = chr.skill(Defs::skill::dexterity);
    auto conVal = chr.skill(Defs::skill::constitution);
    auto intVal = chr.skill(Defs::skill::intelligence);
    auto wisVal = chr.skill(Defs::skill::wisdom);

    currentAge = chr.age();
    years = currentAge - 60 - 1;
    chr.rejuvenate(years);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::old_age);
    REQUIRE(chr.age() == 61);
    REQUIRE(chr.skill(Defs::skill::strength) == strVal + 1);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal + 1);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal + 1);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal - 1);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal - 1);

    strVal = chr.skill(Defs::skill::strength);
    dexVal = chr.skill(Defs::skill::dexterity);
    conVal = chr.skill(Defs::skill::constitution);
    intVal = chr.skill(Defs::skill::intelligence);
    wisVal = chr.skill(Defs::skill::wisdom);

    currentAge = chr.age();
    years = currentAge - 45 - 1;
    chr.rejuvenate(years);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::middle_age);
    REQUIRE(chr.age() == 46);
    REQUIRE(chr.skill(Defs::skill::strength) == strVal + 2);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal + 2);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal + 1);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal - 1);

    strVal = chr.skill(Defs::skill::strength);
    dexVal = chr.skill(Defs::skill::dexterity);
    conVal = chr.skill(Defs::skill::constitution);
    intVal = chr.skill(Defs::skill::intelligence);
    wisVal = chr.skill(Defs::skill::wisdom);

    currentAge = chr.age();
    chr.rejuvenate(20);

    REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
    REQUIRE(chr.age() == 26);
    REQUIRE(chr.skill(Defs::skill::strength) == strVal + 1);
    REQUIRE(chr.skill(Defs::skill::dexterity) == dexVal);
    REQUIRE(chr.skill(Defs::skill::constitution) == conVal + 1);
    REQUIRE(chr.skill(Defs::skill::intelligence) == intVal - 1);
    REQUIRE(chr.skill(Defs::skill::wisdom) == wisVal - 1);
}

static const double get_equipment_weight(const Defs::equipment& equipId)
{
    auto id = static_cast<int>(equipId);
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_equipment, id);
    const QueryResult& res = rs[0];
    auto equipWeight = res.try_or<double>("weight", 0);
    return equipWeight;
}

TEST_CASE("[TC-CHAR.044] Encumbrance affects a character's movement factor", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));

    auto equipWeight = chr.equipment_weight();
    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto backpackId{Defs::equipment::backpack};
    auto backpackWeight = get_equipment_weight(backpackId);
    REQUIRE(chr.add_equipment(backpackId) == true);
    equipWeight += backpackWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto hideId{Defs::equipment::hide};
    auto hideWeight = get_equipment_weight(hideId);
    REQUIRE(chr.add_equipment(hideId) == true);
    equipWeight += hideWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto khopeshId{Defs::equipment::khopesh};
    auto khopeshWeight = get_equipment_weight(khopeshId);
    REQUIRE(chr.add_equipment(khopeshId) == true);
    equipWeight += khopeshWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == 11);

    REQUIRE(chr.remove_equipment(hideId) == true);
    equipWeight -= hideWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);
    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto halberdId{Defs::equipment::halberd};
    auto halberdWeight = get_equipment_weight(halberdId);
    REQUIRE(chr.add_equipment(halberdId, 3) == true);
    equipWeight += halberdWeight * 3;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == 9);

    auto waterClockId{Defs::equipment::water_clock};
    auto waterClockWeight = get_equipment_weight(waterClockId);
    REQUIRE(chr.add_equipment(waterClockId) == true);
    equipWeight += waterClockWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == 0);
}

TEST_CASE("[TC-CHAR.045] Moving equipments between body slots doesn't affect encumbrance", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    Character chr{"Beohram", cls, race, align, sex};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));

    auto equipWeight = chr.equipment_weight();
    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto backpackId{Defs::equipment::backpack};
    auto backpackWeight = get_equipment_weight(backpackId);
    REQUIRE(chr.add_equipment(backpackId) == true);
    equipWeight += backpackWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto equipId{Defs::equipment::scale_mail};
    auto weight = get_equipment_weight(equipId);
    REQUIRE(chr.add_equipment(equipId) == true);
    equipWeight += weight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == 11);
    REQUIRE(chr.move_equipment(equipId, Defs::body_slot::body) == true);
    REQUIRE(chr.movement_factor() == 11);
}

TEST_CASE("[TC-CHAR.046] Encumbrance rule can be deactivated", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;

    OptionalRules::get_instance().option<bool>(Option::apply_encumbrance) = false;
    
    Character chr{"Beohram", cls, race, align, sex};
    chr.change_skill(SkillValue(Defs::skill::strength, 12));

    auto equipWeight = chr.equipment_weight();
    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto backpackId{Defs::equipment::backpack};
    auto backpackWeight = get_equipment_weight(backpackId);
    REQUIRE(chr.add_equipment(backpackId) == true);
    equipWeight += backpackWeight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);

    auto equipId{Defs::equipment::plate_mail};
    auto weight = get_equipment_weight(equipId);
    REQUIRE(chr.add_equipment(equipId) == true);
    equipWeight += weight;
    REQUIRE(chr.equipment_weight() == equipWeight);

    REQUIRE(chr.movement_factor() == Const::high_people_base_movement_factor);
}

TEST_CASE("[TC-CHAR.047] Buy transaction gives a new item to the character", "[character]" )
{
    auto cls = Defs::character_class::fighter;
    auto race = Defs::race::human;
    auto align = Defs::moral_alignment::lawful_neutral;
    auto sex = Defs::sex::male;
    const char* storeName = "Winthrop's store";
    Market::get_instance().add(storeName);
    
    Character chr{"Beohram", cls, race, align, sex};
    chr.add_equipment(Defs::equipment::backpack);

    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = true;

    auto itemId = Defs::equipment::throwing_axe;
    auto& mb = chr.money();
    auto gpAmt = mb[Defs::coin::gold_piece];

    REQUIRE_FALSE(chr.has_equipment_item(itemId, 1));
    REQUIRE(mb[Defs::coin::gold_piece] == gpAmt);
    chr.buy_equipment(storeName, itemId, 1);

    REQUIRE(chr.has_equipment_item(itemId, 1));
    REQUIRE(mb[Defs::coin::gold_piece] < gpAmt);

    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = false;

    Market::get_instance().clear();
    REQUIRE(Market::get_instance().count() == 0);
}