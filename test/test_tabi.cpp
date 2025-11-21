//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>

#include <defs.h>
#include <thief_ability.h>
#include <common_types.h>
#include <adnd_errors.h>

using namespace Adndtk;

static std::vector<Defs::thief_ability> thievingAbilities =
        {Defs::thief_ability::pick_pockets, Defs::thief_ability::open_locks,
        Defs::thief_ability::find_remove_traps, Defs::thief_ability::move_silently,
        Defs::thief_ability::hide_in_shadows, Defs::thief_ability::hear_noise,
        Defs::thief_ability::climb_walls, Defs::thief_ability::read_languages};

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

static ThievingScore baseScore(const Defs::race& r, const Defs::thief_ability& t, const short& dexValue, const std::optional<Defs::equipment>& armour)
{
    auto baseScore = baseScores[t]
                + raceModifiers[r][t]
                + dexterityModifiers[dexValue][t]
                + armourModifiers[armour][t];
    return baseScore;
}

static void checkThiefAbilities(const ThiefAbility& tAbil, const Defs::race& r, const std::vector<ThievingScore>& skl, const std::optional<Defs::equipment>& armour)
{
    auto dexValue = tAbil.dexterity_value();
    for (auto t : thievingAbilities)
    {
        auto val = std::min<ThievingScore>(95, skl[static_cast<short>(t)] + baseScore(r, t, dexValue, armour));
        REQUIRE(tAbil[t] == val);
    }
}

TEST_CASE("[TC-TABI.001] Skill modifiers are applied according to the race", "[thief_ability]" )
{
    for (auto r : {Defs::race::human, Defs::race::dwarf, Defs::race::elf, Defs::race::gnome,
                Defs::race::half_elf, Defs::race::halfling})
    {
        ThiefAbility tAbil{r};
        tAbil.dexterity_change(13); // Dexterity values between 13 and 15 gives no bonus/malus

        for (auto rm : raceModifiers[r])
        for (auto t : thievingAbilities)
        {
            REQUIRE(tAbil[t] == baseScore(r, t, tAbil.dexterity_value(), tAbil.armour_in_use()));
        }
    }
}

TEST_CASE("[TC-TABI.002] Skill bonuses/maluses are applied according to the thief's dexterity score", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    for (auto dm : dexterityModifiers)
    {
        short dexValue = dm.first;
        tAbil.dexterity_change(dexValue);
        for (auto t : thievingAbilities)
        {
            REQUIRE(tAbil[t] == baseScore(r, t, tAbil.dexterity_value(), tAbil.armour_in_use()));
        }
    }
}

TEST_CASE("[TC-TABI.003] Skill bonuses/maluses are applied according to the thief's armour in use", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};
    tAbil.dexterity_change(13); // Dexterity values between 13 and 15 gives no bonus/malus

    for (auto am : armourModifiers)
    {
        auto armourId = am.first;
        tAbil.armour_change(armourId);
        for (auto t : thievingAbilities)
        {
            REQUIRE(tAbil[t] == baseScore(r, t, tAbil.dexterity_value(), armourId));
        }
    }
}

TEST_CASE("[TC-TABI.004] Improving level makes new ability slots available", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.level() == 1);
    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.level() == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
}

TEST_CASE("[TC-TABI.005] Improving abilities consume available slots", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 2);
}

TEST_CASE("[TC-TABI.006] Using less then 60 points at 1st level to upgrade skills is fine", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    tAbil.improve_abilities(7,7,7,7,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 2);
}

TEST_CASE("[TC-TABI.007] Using more then 60 points at 1st level to upgrade skills is not fine", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(8,8,8,8,7,7,7,8), SkillException);
    REQUIRE(tAbil.available_ability_slots() == 3);
}

TEST_CASE("[TC-TABI.008] Skills cannt be assigned more then 30 points at 1st level to upgrade skills", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(32,4,4,4,4,4,4,4), SkillException);
    REQUIRE(tAbil.available_ability_slots() == 3);
}

TEST_CASE("[TC-TABI.009] Using more then 30 points at 2st level to upgrade skills is not fine", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 2);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(4,4,4,4,4,4,4,4), SkillException);
    REQUIRE(tAbil.available_ability_slots() == 2);
}

TEST_CASE("[TC-TABI.010] Skills cannot be assigned more then 15 points at 1st level to upgrade skills", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    REQUIRE(tAbil.level_change(3) == 3);
    REQUIRE(tAbil.available_ability_slots() == 3);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 2);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(16,3,2,2,2,2,2,2), SkillException);
    REQUIRE(tAbil.available_ability_slots() == 2);
}

TEST_CASE("[TC-TABI.011] Thieves cannot improve abilities beyond the given slots availability", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 0);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(4,4,4,4,4,4,3,3), SkillException);
    REQUIRE(tAbil.available_ability_slots() == 0);
}

TEST_CASE("[TC-TABI.012] When thieves loose and regain levels, skills are restored", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 0);
    REQUIRE(tAbil.level_change(3) == 2);
    REQUIRE(tAbil.available_ability_slots() == 2);
    tAbil.improve_abilities(4,4,4,4,4,4,3,3);
    REQUIRE(tAbil.available_ability_slots() == 1);

    std::vector<ThievingScore> skl1{8, 8, 8, 8, 7, 7, 7, 7};
    std::vector<ThievingScore> skl2{12, 12, 12, 12, 11, 11, 10, 10};
    
    checkThiefAbilities(tAbil, r, skl2, tAbil.armour_in_use());

    REQUIRE(tAbil.level_change(1) == 0);
    REQUIRE(tAbil.available_ability_slots() == 0);

    checkThiefAbilities(tAbil, r, skl1, tAbil.armour_in_use());

    REQUIRE(tAbil.level_change(2) == 0);
    REQUIRE(tAbil.available_ability_slots() == 0);

    checkThiefAbilities(tAbil, r, skl2, tAbil.armour_in_use());
}

TEST_CASE("[TC-TABI.013] When level is regained, skills are restored, no further improve is possible", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.available_ability_slots() == 1);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    REQUIRE(tAbil.available_ability_slots() == 0);
    REQUIRE(tAbil.level_change(2) == 1);
    REQUIRE(tAbil.available_ability_slots() == 1);
    tAbil.improve_abilities(4,4,4,4,4,4,3,3);
    REQUIRE(tAbil.available_ability_slots() == 0);

    tAbil.level_change(1);
    REQUIRE(tAbil.available_ability_slots() == 0);
    tAbil.level_change(2);
    REQUIRE(tAbil.available_ability_slots() == 0);
    REQUIRE_THROWS_AS(tAbil.improve_abilities(4,4,4,4,4,4,3,3), SkillException);
}

TEST_CASE("[TC-TABI.014] Skills cannot exceed 95%", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};

    REQUIRE(tAbil.level_change(Const::max_character_level) == Const::max_character_level);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    for (ExperienceLevel el=0; el<tAbil.level()-1; ++el)
    {
        tAbil.improve_abilities(4,4,4,4,4,4,3,3);
    }

    std::vector<ThievingScore> skl{84, 84, 84, 84, 83, 83, 64, 64};
    checkThiefAbilities(tAbil, r, skl, tAbil.armour_in_use());
}

TEST_CASE("[TC-TABI.015] Skills can have negative values", "[thief_ability]" )
{
    Defs::race r{Defs::race::dwarf};
    ThiefAbility tAbil{r};
    tAbil.dexterity_change(9);
    tAbil.armour_change(Defs::equipment::elven_chain_mail);

    tAbil.improve_abilities(8,8,8,8,7,7,7,7);

    std::vector<ThievingScore> skl{-12, 13, 13, -12, -8, 17, 37, 2};
    auto dexValue = tAbil.dexterity_value();
    for (auto t : thievingAbilities)
    {
        REQUIRE(tAbil[t] == skl[static_cast<short>(t)]);
    }
}

TEST_CASE("[TC-TABI.016] Bonus/malus is considered before cutting the score to 95%", "[thief_ability]" )
{
    Defs::race r{Defs::race::human};
    ThiefAbility tAbil{r};
    tAbil.dexterity_change(13);

    REQUIRE(tAbil.level_change(Const::max_character_level) == Const::max_character_level);
    tAbil.improve_abilities(8,8,8,8,7,7,7,7);
    for (ExperienceLevel el=0; el<tAbil.level()-1; ++el)
    {
        tAbil.improve_abilities(4,4,4,4,4,4,3,3);
    }

    short malus = -5;
    Defs::thief_ability pp = Defs::thief_ability::pick_pockets;
    REQUIRE(tAbil[pp] == 95);
    REQUIRE(tAbil.get(pp, malus) == 95);

    Defs::thief_ability ol = Defs::thief_ability::open_locks;
    REQUIRE(tAbil[ol] == 94);
    REQUIRE(tAbil.get(ol, malus) == 89);

    short bonus = 10;
    Defs::thief_ability frt = Defs::thief_ability::find_remove_traps;
    REQUIRE(tAbil[frt] == 89);
    REQUIRE(tAbil.get(frt, bonus) == 95);
}