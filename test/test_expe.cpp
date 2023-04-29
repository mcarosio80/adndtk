//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

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