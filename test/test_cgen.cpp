//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include <optional>
#include <algorithm>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-CGEN.001] Human race is always available, no matter the skill values", "[character_generator]" )
{
    SkillValue str = SkillCreator::create(Defs::skill::strength, std::nullopt, std::nullopt);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, std::nullopt, std::nullopt);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, std::nullopt, std::nullopt);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, std::nullopt, std::nullopt);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, std::nullopt, std::nullopt);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, std::nullopt, std::nullopt);

    auto races = CharacterGenerator::available_races(str, dex, con, inl, wis, cha);
    auto pred = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::human;
    };
    auto found = std::find_if(races.begin(), races.end(), pred) != races.end();
    REQUIRE(found);
}

TEST_CASE("[TC-CGEN.002] Dwarf race requires skills Str: 8-18, Dex: 3-17, Con: 11-18, Int: 3-18, Wis: 3-18, Cha: 3-17", "[character_generator]" )
{
    auto raceAllowed = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::dwarf;
    };
    std::vector<Tables::race> races{}; 

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 5}
    );
    REQUIRE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 7},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 5}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 5}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 5}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 15},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 18}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());
}

TEST_CASE("[TC-CGEN.003] Elf race requires skills Str: 3-18, Dex: 6-18, Con: 7-18, Int: 8-18, Wis: 3-18, Cha: 8-18", "[character_generator]" )
{
    auto raceAllowed = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::elf;
    };
    std::vector<Tables::race> races{}; 

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 7},
        SkillValue{Defs::skill::dexterity, 5},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 6},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());
}

TEST_CASE("[TC-CGEN.004] Gnome race requires skills Str: 6-18, Dex: 3-18, Con: 8-18, Int: 6-18, Wis: 3-18, Cha: 3-18", "[character_generator]" )
{
    auto raceAllowed = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::gnome;
    };
    std::vector<Tables::race> races{}; 

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 5},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 6},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 5},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());
}

TEST_CASE("[TC-CGEN.005] Half-elf race requires skills Str: 3-18, Dex: 6-18, Con: 6-18, Int: 4-18, Wis: 3-18, Cha: 3-18", "[character_generator]" )
{
    auto raceAllowed = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::half_elf;
    };
    std::vector<Tables::race> races{}; 

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 5},
        SkillValue{Defs::skill::dexterity, 5},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 5},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 3},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());
}

TEST_CASE("[TC-CGEN.006] Halfling race requires skills Str: 7-18, Dex: 7-18, Con: 10-18, Int: 6-18, Wis: 3-17, Cha: 3-18", "[character_generator]" )
{
    auto raceAllowed = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::halfling;
    };
    std::vector<Tables::race> races{}; 

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 6},
        SkillValue{Defs::skill::dexterity, 15},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 6},
        SkillValue{Defs::skill::constitution, 5},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 15}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 13},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 18},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 5},
        SkillValue{Defs::skill::wisdom, 11},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());

    races = CharacterGenerator::available_races(
        SkillValue{Defs::skill::strength, 17},
        SkillValue{Defs::skill::dexterity, 14},
        SkillValue{Defs::skill::constitution, 9},
        SkillValue{Defs::skill::intelligence, 15},
        SkillValue{Defs::skill::wisdom, 18},
        SkillValue{Defs::skill::charisma, 7}
    );
    REQUIRE_FALSE(std::find_if(races.begin(), races.end(), raceAllowed) != races.end());
}

TEST_CASE("[TC-CGEN.007] Fighters require skills Str: 9+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::fighter;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 8},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.008] Paladin require skills Str: 12+, Con: 9+, Wis: 13+, Cha: 17+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::paladin;
    };
    std::vector<Tables::character_class> classes{}; 

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 12},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 18},
        Defs::race::human
    );
    REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 11},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 18},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 15},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 8},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 18},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 15},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 12},
        SkillValue{Defs::skill::charisma, 18},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 15},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 12},
        SkillValue{Defs::skill::wisdom, 152},
        SkillValue{Defs::skill::charisma, 16},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
}

TEST_CASE("[TC-CGEN.009] Demi-humans cannot be paladins", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::paladin;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::elf, Defs::race::dwarf, Defs::race::gnome,
                        Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 16},
            SkillValue{Defs::skill::dexterity, 14},
            SkillValue{Defs::skill::constitution, 15},
            SkillValue{Defs::skill::intelligence, 14},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 18},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.010] Rangers require skills Str: 13+, Dex: 13+, Con: 14+, Wis: 14+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::ranger;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 12},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.011] Dwarves, Gnomes and Halflings cannot be rangers", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::ranger;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 16},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 16},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.012] Mages require skills Int: 9+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::mage;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 8},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.013] Dwarves, Gnomes and Halflings cannot be mages", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::mage;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.014] Abjurers require skills Int: 9+, Wis: 15+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::abjurer;
    };
    std::vector<Tables::character_class> classes{}; 

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 12},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 16},
        SkillValue{Defs::skill::intelligence, 15},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 14},
        Defs::race::human
    );
    REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 12},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 16},
        SkillValue{Defs::skill::intelligence, 8},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 14},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 12},
        SkillValue{Defs::skill::dexterity, 12},
        SkillValue{Defs::skill::constitution, 16},
        SkillValue{Defs::skill::intelligence, 15},
        SkillValue{Defs::skill::wisdom, 14},
        SkillValue{Defs::skill::charisma, 14},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
}

TEST_CASE("[TC-CGEN.015] Demi-humans cannot be abjurers", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::abjurer;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::elf, Defs::race::dwarf, Defs::race::gnome,
                        Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 16},
            SkillValue{Defs::skill::dexterity, 14},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 17},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.016] Conjurers require skills Int: 9+, Con: 15+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::conjurer;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 15},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.017] Dwarves, Elves, Gnomes and Halflings cannot be conjurers", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::conjurer;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 12},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 16},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.018] Diviners require skills Int: 9+, Wis: 16+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::diviner;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 17},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 17},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 14},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.019] Dwarves, Gnomes and Halflings cannot be diviners", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::diviner;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 12},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 16},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 16},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.020] Enchanters require skills Int: 9+, Cha: 16+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::enchanter;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 16},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 16},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.021] Dwarves, Gnomes and Halflings cannot be enchanters", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::enchanter;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 17},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.022] Illusioninsts require skills Int: 9+, Dex: 16+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::illusionist;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::gnome})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 16},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::gnome})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 16},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::gnome})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.023] Dwarves, Elves, Half-elves and Halflings cannot be illusionists", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::illusionist;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 16},
            SkillValue{Defs::skill::constitution, 13},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.024] Invokers require skills Int: 9+, Con: 16+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::invoker;
    };
    std::vector<Tables::character_class> classes{}; 

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 16},
        SkillValue{Defs::skill::intelligence, 16},
        SkillValue{Defs::skill::wisdom, 13},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 16},
        SkillValue{Defs::skill::intelligence, 8},
        SkillValue{Defs::skill::wisdom, 13},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 15},
        SkillValue{Defs::skill::intelligence, 16},
        SkillValue{Defs::skill::wisdom, 13},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
}

TEST_CASE("[TC-CGEN.025] Demi-humans cannot be invokers", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::invoker;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::half_elf,
                    Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 163},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.026] Necromancers require skills Int: 9+, Wis: 16+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::necromancer;
    };
    std::vector<Tables::character_class> classes{}; 

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 16},
        SkillValue{Defs::skill::wisdom, 16},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 8},
        SkillValue{Defs::skill::wisdom, 16},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());

    classes = CharacterGenerator::available_classes(
        SkillValue{Defs::skill::strength, 14},
        SkillValue{Defs::skill::dexterity, 13},
        SkillValue{Defs::skill::constitution, 14},
        SkillValue{Defs::skill::intelligence, 16},
        SkillValue{Defs::skill::wisdom, 15},
        SkillValue{Defs::skill::charisma, 12},
        Defs::race::human
    );
    REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
}

TEST_CASE("[TC-CGEN.027] Demi-humans cannot be necromancers", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::necromancer;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::half_elf,
                    Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 13},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 16},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.028] Transmuters require skills Int: 9+, Dex: 15+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::transmuter;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 8},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 14},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.029] Dwarves, Elves, Gnomes and Halflings cannot be transmuters", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::transmuter;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 16},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 16},
            SkillValue{Defs::skill::wisdom, 13},
            SkillValue{Defs::skill::charisma, 12},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.030] Clerics require skills Wis: 9+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::cleric;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 8},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.031] Preists of specific mythos require skills Wis: 9+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::preist_of_specific_mythos;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 8},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.032] Druids require skills Wis: 12+, Cha: 15+", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::druid;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 12},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 12},
            SkillValue{Defs::skill::charisma, 14},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.033] Dwarves, Elves, Gnomes and Halflings cannot be druids", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::druid;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::dwarf, Defs::race::elf, Defs::race::gnome, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 14},
            SkillValue{Defs::skill::dexterity, 15},
            SkillValue{Defs::skill::constitution, 12},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 16},
            SkillValue{Defs::skill::charisma, 16},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.034] Thiefs require skills Dex: 9+, plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::thief;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::elf, Defs::race::dwarf,
                    Defs::race::gnome, Defs::race::half_elf, Defs::race::halfling})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 8},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 8},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}

TEST_CASE("[TC-CGEN.034] Bards require skills Dex: 12+, Int: 13+, Cha: 15+ plus race requisites", "[character_generator]" )
{
    auto classAllowed = [](const Tables::character_class& c) -> bool
    {
        return static_cast<Defs::character_class>(c.id) == Defs::character_class::bard;
    };
    std::vector<Tables::character_class> classes{}; 

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 14},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 11},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 14},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 12},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 15},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }

    for (auto& race : {Defs::race::human, Defs::race::half_elf})
    {
        classes = CharacterGenerator::available_classes(
            SkillValue{Defs::skill::strength, 11},
            SkillValue{Defs::skill::dexterity, 12},
            SkillValue{Defs::skill::constitution, 14},
            SkillValue{Defs::skill::intelligence, 14},
            SkillValue{Defs::skill::wisdom, 11},
            SkillValue{Defs::skill::charisma, 14},
            race
        );
        REQUIRE_FALSE(std::find_if(classes.begin(), classes.end(), classAllowed) != classes.end());
    }
}