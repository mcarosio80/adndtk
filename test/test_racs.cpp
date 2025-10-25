//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <map>

#include <defs.h>
#include <racial_stats.h>

using namespace Adndtk;

TEST_CASE("[TC-RACS.001] Humans have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 150}, {Defs::sex::female, 148} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 63}, {Defs::sex::female, 45} };
    const short baseAge{15};
    const short maxAge{90};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 45 },
        { Defs::aging_effects::old_age, 60 },
        { Defs::aging_effects::venerable, 90 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::human, s};
        REQUIRE(chr.height() >= baseHeight[s] + 5);
        REQUIRE(chr.height() <= baseHeight[s] + 50);
        REQUIRE(chr.weight() >= baseWeight[s] + 3);
        REQUIRE(chr.weight() <= baseWeight[s] + 30);
        REQUIRE(chr.current_age() >= baseAge + 1);
        REQUIRE(chr.current_age() <= baseAge + 4);
        REQUIRE(chr.max_age() >= maxAge + 2);
        REQUIRE(chr.max_age() <= maxAge + 40);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}

TEST_CASE("[TC-RACS.002] Dwarves have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 108}, {Defs::sex::female, 103} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 59}, {Defs::sex::female, 48} };
    const short baseAge{40};
    const short maxAge{250};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 125 },
        { Defs::aging_effects::old_age, 167 },
        { Defs::aging_effects::venerable, 250 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::dwarf, s};
        REQUIRE(chr.height() >= baseHeight[s] + 3);
        REQUIRE(chr.height() <= baseHeight[s] + 25);
        REQUIRE(chr.weight() >= baseWeight[s] + 2);
        REQUIRE(chr.weight() <= baseWeight[s] + 20);
        REQUIRE(chr.current_age() >= baseAge + 5);
        REQUIRE(chr.current_age() <= baseAge + 30);
        REQUIRE(chr.max_age() >= maxAge + 2);
        REQUIRE(chr.max_age() <= maxAge + 200);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}

TEST_CASE("[TC-RACS.003] Elves have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 140}, {Defs::sex::female, 130} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 41}, {Defs::sex::female, 38} };
    const short baseAge{100};
    const short maxAge{350};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 175 },
        { Defs::aging_effects::old_age, 233 },
        { Defs::aging_effects::venerable, 350 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::elf, s};
        REQUIRE(chr.height() >= baseHeight[s] + 3);
        REQUIRE(chr.height() <= baseHeight[s] + 25);
        REQUIRE(chr.weight() >= baseWeight[s] + 2);
        REQUIRE(chr.weight() <= baseWeight[s] + 15);
        REQUIRE(chr.current_age() >= baseAge + 5);
        REQUIRE(chr.current_age() <= baseAge + 30);
        REQUIRE(chr.max_age() >= maxAge + 4);
        REQUIRE(chr.max_age() <= maxAge + 400);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}

TEST_CASE("[TC-RACS.004] Gnomes have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 95}, {Defs::sex::female, 90} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 33}, {Defs::sex::female, 31} };
    const short baseAge{60};
    const short maxAge{200};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 100 },
        { Defs::aging_effects::old_age, 133 },
        { Defs::aging_effects::venerable, 200 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::gnome, s};
        REQUIRE(chr.height() >= baseHeight[s] + 3);
        REQUIRE(chr.height() <= baseHeight[s] + 25);
        REQUIRE(chr.weight() >= baseWeight[s] + 3);
        REQUIRE(chr.weight() <= baseWeight[s] + 10);
        REQUIRE(chr.current_age() >= baseAge + 3);
        REQUIRE(chr.current_age() <= baseAge + 36);
        REQUIRE(chr.max_age() >= maxAge + 3);
        REQUIRE(chr.max_age() <= maxAge + 300);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}

TEST_CASE("[TC-RACS.005] Half-elves have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 150}, {Defs::sex::female, 145} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 50}, {Defs::sex::female, 39} };
    const short baseAge{15};
    const short maxAge{125};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 62 },
        { Defs::aging_effects::old_age, 83 },
        { Defs::aging_effects::venerable, 125 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::half_elf, s};
        REQUIRE(chr.height() >= baseHeight[s] + 5);
        REQUIRE(chr.height() <= baseHeight[s] + 30);
        REQUIRE(chr.weight() >= baseWeight[s] + 2);
        REQUIRE(chr.weight() <= baseWeight[s] + 18);
        REQUIRE(chr.current_age() >= baseAge + 1);
        REQUIRE(chr.current_age() <= baseAge + 6);
        REQUIRE(chr.max_age() >= maxAge + 3);
        REQUIRE(chr.max_age() <= maxAge + 60);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}

TEST_CASE("[TC-RACS.006] Halflings have racial stats between bundaries", "[racial_stats]" )
{
    std::map<Defs::sex, short> baseHeight{ {Defs::sex::male, 80}, {Defs::sex::female, 75} };
    std::map<Defs::sex, short> baseWeight{ {Defs::sex::male, 24}, {Defs::sex::female, 22} };
    const short baseAge{20};
    const short maxAge{100};

    std::map<Defs::aging_effects, short> aging {
        { Defs::aging_effects::middle_age, 50 },
        { Defs::aging_effects::old_age, 67 },
        { Defs::aging_effects::venerable, 100 }
    };

    for (const auto& s : {Defs::sex::male, Defs::sex::female})
    {
        RacialStats chr{Defs::race::halfling, s};
        REQUIRE(chr.height() >= baseHeight[s] + 5);
        REQUIRE(chr.height() <= baseHeight[s] + 40);
        REQUIRE(chr.weight() >= baseWeight[s] + 3);
        REQUIRE(chr.weight() <= baseWeight[s] + 10);
        REQUIRE(chr.current_age() >= baseAge + 3);
        REQUIRE(chr.current_age() <= baseAge + 12);
        REQUIRE(chr.max_age() >= maxAge + 1);
        REQUIRE(chr.max_age() <= maxAge + 100);

        REQUIRE(chr.get_age_range() == Defs::aging_effects::young);
        for (const auto& a : { Defs::aging_effects::middle_age,
                            Defs::aging_effects::old_age,
                            Defs::aging_effects::venerable})
        {
            auto age = aging[a] - chr.current_age();
            chr.grow_old(age);
            REQUIRE(chr.get_age_range() == a);
        }
    }
}