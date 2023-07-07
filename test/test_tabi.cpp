//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include <optional>

#include <adndtk.h>

using namespace Adndtk;

static std::map<Defs::thief_ability, ThievingScore> baseScores = 
{
    {Defs::thief_ability::pick_pockets, 15},
    {Defs::thief_ability::open_locks, 10},
    {Defs::thief_ability::find_remove_traps, 5},
    {Defs::thief_ability::move_silently, 10},
    {Defs::thief_ability::hide_in_shadows, 5},
    {Defs::thief_ability::hear_noise, 15},
    {Defs::thief_ability::climb_walls, 60},
    {Defs::thief_ability::read_languages, 0}
};

static std::map<Defs::race, std::map<Defs::thief_ability, ThievingScore>> raceModifiers = 
{
    {Defs::race::human, {
        {Defs::thief_ability::pick_pockets, 0},
        {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0},
        {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0},
        {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0},
        {Defs::thief_ability::read_languages, 0}
    }},
    {Defs::race::dwarf, {
        {Defs::thief_ability::pick_pockets, 0},
        {Defs::thief_ability::open_locks, 10},
        {Defs::thief_ability::find_remove_traps, 15},
        {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0},
        {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, -10},
        {Defs::thief_ability::read_languages, -5}
    }},
    {Defs::race::elf, {
        {Defs::thief_ability::pick_pockets, 5},
        {Defs::thief_ability::open_locks, -5},
        {Defs::thief_ability::find_remove_traps, 0},
        {Defs::thief_ability::move_silently, 5},
        {Defs::thief_ability::hide_in_shadows, 10},
        {Defs::thief_ability::hear_noise, 5},
        {Defs::thief_ability::climb_walls, 0},
        {Defs::thief_ability::read_languages, 0}
    }},
    {Defs::race::gnome, {
        {Defs::thief_ability::pick_pockets, 0},
        {Defs::thief_ability::open_locks, 5},
        {Defs::thief_ability::find_remove_traps, 10},
        {Defs::thief_ability::move_silently, 5},
        {Defs::thief_ability::hide_in_shadows, 5},
        {Defs::thief_ability::hear_noise, 10},
        {Defs::thief_ability::climb_walls, -15},
        {Defs::thief_ability::read_languages, 0}
    }},
    {Defs::race::half_elf, {
        {Defs::thief_ability::pick_pockets, 10},
        {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0},
        {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 5},
        {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0},
        {Defs::thief_ability::read_languages, 0}
    }},
    {Defs::race::halfling, {
        {Defs::thief_ability::pick_pockets, 5},
        {Defs::thief_ability::open_locks, 5},
        {Defs::thief_ability::find_remove_traps, 5},
        {Defs::thief_ability::move_silently, 10},
        {Defs::thief_ability::hide_in_shadows, 15},
        {Defs::thief_ability::hear_noise, 5},
        {Defs::thief_ability::climb_walls, -15},
        {Defs::thief_ability::read_languages, -5}
    }}
};

static std::map<short, std::map<Defs::thief_ability, ThievingScore>> dexterityModifiers = 
{
    {9, {
        {Defs::thief_ability::pick_pockets, -15}, {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10}, {Defs::thief_ability::move_silently, -20},
        {Defs::thief_ability::hide_in_shadows, -10}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {10, {
        {Defs::thief_ability::pick_pockets, -10}, {Defs::thief_ability::open_locks, -5},
        {Defs::thief_ability::find_remove_traps, -10}, {Defs::thief_ability::move_silently, -15},
        {Defs::thief_ability::hide_in_shadows, -5}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {11, {
        {Defs::thief_ability::pick_pockets, -5}, {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, -5}, {Defs::thief_ability::move_silently, -10},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {12, {
        {Defs::thief_ability::pick_pockets, 0}, {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, -5},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {13, {
        {Defs::thief_ability::pick_pockets, 0}, {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {14, {
        {Defs::thief_ability::pick_pockets, 0}, {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {15, {
        {Defs::thief_ability::pick_pockets, 0}, {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {16, {
        {Defs::thief_ability::pick_pockets, 0}, {Defs::thief_ability::open_locks, 5},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, 0},
        {Defs::thief_ability::hide_in_shadows, 0}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {17, {
        {Defs::thief_ability::pick_pockets, 5}, {Defs::thief_ability::open_locks, 10},
        {Defs::thief_ability::find_remove_traps, 0}, {Defs::thief_ability::move_silently, 5},
        {Defs::thief_ability::hide_in_shadows, 5}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {18, {
        {Defs::thief_ability::pick_pockets, 10}, {Defs::thief_ability::open_locks, 15},
        {Defs::thief_ability::find_remove_traps, 5}, {Defs::thief_ability::move_silently, 10},
        {Defs::thief_ability::hide_in_shadows, 10}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }},
    {19, {
        {Defs::thief_ability::pick_pockets, 15}, {Defs::thief_ability::open_locks, 20},
        {Defs::thief_ability::find_remove_traps, 10}, {Defs::thief_ability::move_silently, 15},
        {Defs::thief_ability::hide_in_shadows, 15}, {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 0}, {Defs::thief_ability::read_languages, 0}
    }}
};

static std::map<std::optional<Defs::equipment>, std::map<Defs::thief_ability, ThievingScore>> armourModifiers = 
{
    {std::nullopt, {
        {Defs::thief_ability::pick_pockets, 5},
        {Defs::thief_ability::open_locks, 0},
        {Defs::thief_ability::find_remove_traps, 0},
        {Defs::thief_ability::move_silently, 10},
        {Defs::thief_ability::hide_in_shadows, 5},
        {Defs::thief_ability::hear_noise, 0},
        {Defs::thief_ability::climb_walls, 10},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::elven_chain_mail), {
        {Defs::thief_ability::pick_pockets, -20},
        {Defs::thief_ability::open_locks, -5},
        {Defs::thief_ability::find_remove_traps, -5},
        {Defs::thief_ability::move_silently, -10},
        {Defs::thief_ability::hide_in_shadows, -10},
        {Defs::thief_ability::hear_noise, -5},
        {Defs::thief_ability::climb_walls, -20},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::padded), {
        {Defs::thief_ability::pick_pockets, -30},
        {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10},
        {Defs::thief_ability::move_silently, -20},
        {Defs::thief_ability::hide_in_shadows, -20},
        {Defs::thief_ability::hear_noise, -10},
        {Defs::thief_ability::climb_walls, -30},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::hide), {
        {Defs::thief_ability::pick_pockets, -30},
        {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10},
        {Defs::thief_ability::move_silently, -20},
        {Defs::thief_ability::hide_in_shadows, -20},
        {Defs::thief_ability::hear_noise, -10},
        {Defs::thief_ability::climb_walls, -30},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::studded_leather), {
        {Defs::thief_ability::pick_pockets, -30},
        {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10},
        {Defs::thief_ability::move_silently, -20},
        {Defs::thief_ability::hide_in_shadows, -20},
        {Defs::thief_ability::hear_noise, -10},
        {Defs::thief_ability::climb_walls, -30},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::chain_mail), {
        {Defs::thief_ability::pick_pockets, -25},
        {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10},
        {Defs::thief_ability::move_silently, -15},
        {Defs::thief_ability::hide_in_shadows, -15},
        {Defs::thief_ability::hear_noise, -5},
        {Defs::thief_ability::climb_walls, -25},
        {Defs::thief_ability::read_languages, 0}
    }},
    {std::optional<Defs::equipment>(Defs::equipment::ring_mail), {
        {Defs::thief_ability::pick_pockets, -25},
        {Defs::thief_ability::open_locks, -10},
        {Defs::thief_ability::find_remove_traps, -10},
        {Defs::thief_ability::move_silently, -15},
        {Defs::thief_ability::hide_in_shadows, -15},
        {Defs::thief_ability::hear_noise, -5},
        {Defs::thief_ability::climb_walls, -25},
        {Defs::thief_ability::read_languages, 0}
    }}
};

TEST_CASE("[TC-TABI.001] Skill modifiers are applied according to the race", "[thief_ability]" )
{
    for (auto r : {Defs::race::human, Defs::race::dwarf, Defs::race::elf, Defs::race::gnome,
                Defs::race::half_elf, Defs::race::halfling})
    {
        ThiefAbility tAbil{r};
        tAbil.dexterity_change(13); // Dexterity values between 13 and 15 gives no bonus/malus

        for (auto rm : raceModifiers[r])
        for (auto t : {Defs::thief_ability::pick_pockets, Defs::thief_ability::open_locks,
                    Defs::thief_ability::find_remove_traps, Defs::thief_ability::move_silently,
                    Defs::thief_ability::hide_in_shadows, Defs::thief_ability::hear_noise,
                    Defs::thief_ability::climb_walls, Defs::thief_ability::read_languages})
        {
            REQUIRE(tAbil[t] == baseScores[t] + raceModifiers[r][t] + armourModifiers[std::nullopt][t]);
        }
    }
}

TEST_CASE("[TC-TABI.002] Skill bonuses/maluses are applied according to the thief's dexterity score", "[thief_ability]" )
{
    ThiefAbility tAbil{Defs::race::human};

    for (auto dm : dexterityModifiers)
    {
        short dexValue = dm.first;
        tAbil.dexterity_change(dexValue);
        for (auto t : {Defs::thief_ability::pick_pockets, Defs::thief_ability::open_locks,
                        Defs::thief_ability::find_remove_traps, Defs::thief_ability::move_silently,
                        Defs::thief_ability::hide_in_shadows, Defs::thief_ability::hear_noise,
                        Defs::thief_ability::climb_walls, Defs::thief_ability::read_languages})
        {
            REQUIRE(tAbil[t] == baseScores[t] + dexterityModifiers[dexValue][t] + armourModifiers[std::nullopt][t]);
        }
    }
}

TEST_CASE("[TC-TABI.003] Skill bonuses/maluses are applied according to the thief's armour in use", "[thief_ability]" )
{
    ThiefAbility tAbil{Defs::race::human};
    tAbil.dexterity_change(13); // Dexterity values between 13 and 15 gives no bonus/malus

    for (auto am : armourModifiers)
    {
        auto armourId = am.first;
        tAbil.armour_change(armourId);
        for (auto t : {Defs::thief_ability::pick_pockets, Defs::thief_ability::open_locks,
                        Defs::thief_ability::find_remove_traps, Defs::thief_ability::move_silently,
                        Defs::thief_ability::hide_in_shadows, Defs::thief_ability::hear_noise,
                        Defs::thief_ability::climb_walls, Defs::thief_ability::read_languages})
        {
            REQUIRE(tAbil[t] == baseScores[t] + armourModifiers[armourId][t]);
        }
    }
}

// void improve_abilities(const std::vector<ThievingScore>& scores);

// void improve_abilities(const ThievingScore& pickPockets, const ThievingScore& openLocks, const ThievingScore& findRemoveTraps,
//                     const ThievingScore& moveSilently, const ThievingScore& hideInShadows, const ThievingScore& hearNoise,
//                     const ThievingScore& climbWalls, const ThievingScore& readLanguages);
