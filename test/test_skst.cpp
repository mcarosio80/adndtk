//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <skill_stats.h>

using namespace Adndtk;

TEST_CASE("[TC-SKST.001] Strength statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::strength;
    {
        SkillValue skl{sklId, 18, 48};
        auto sklStat = SkillStats::get_instance().get_strength_stats(skl);

        REQUIRE(sklStat.hit_modifier == 1);
        REQUIRE(sklStat.damage_adjustment == 3);
        REQUIRE(sklStat.weight_allowance == 61);
        REQUIRE(sklStat.maximum_press == 127);
        REQUIRE(sklStat.open_doors == 12);
        REQUIRE_FALSE(sklStat.open_doors_special.has_value());
        REQUIRE(sklStat.bend_bars_lift_gates == 20);
    }
    {
        SkillValue skl{sklId, 5};
        auto sklStat = SkillStats::get_instance().get_strength_stats(skl);

        REQUIRE(sklStat.hit_modifier == -2);
        REQUIRE(sklStat.damage_adjustment == -1);
        REQUIRE(sklStat.weight_allowance == 4.5);
        REQUIRE(sklStat.maximum_press == 11);
        REQUIRE(sklStat.open_doors == 3);
        REQUIRE_FALSE(sklStat.open_doors_special.has_value());
        REQUIRE(sklStat.bend_bars_lift_gates == 0);
    }
    {
        SkillValue skl{sklId, 23};
        auto sklStat = SkillStats::get_instance().get_strength_stats(skl);

        REQUIRE(sklStat.hit_modifier == 5);
        REQUIRE(sklStat.damage_adjustment == 11);
        REQUIRE(sklStat.weight_allowance == 423.5);
        REQUIRE(sklStat.maximum_press == 512);
        REQUIRE(sklStat.open_doors == 18);
        REQUIRE(sklStat.open_doors_special.has_value());
        REQUIRE(sklStat.open_doors_special.value() == 16);
        REQUIRE(sklStat.bend_bars_lift_gates == 90);
    }
}

TEST_CASE("[TC-SKST.002] Dexterity statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::dexterity;
    {
        SkillValue skl{sklId, 4};
        auto sklStat = SkillStats::get_instance().get_dexterity_stats(skl);

        REQUIRE(sklStat.reaction_adjustment == -2);
        REQUIRE(sklStat.missile_attack_adjustment == -2);
        REQUIRE(sklStat.defensive_adjustment == 3);
    }
    {
        SkillValue skl{sklId, 15};
        auto sklStat = SkillStats::get_instance().get_dexterity_stats(skl);

        REQUIRE(sklStat.reaction_adjustment == 0);
        REQUIRE(sklStat.missile_attack_adjustment == 0);
        REQUIRE(sklStat.defensive_adjustment == -1);
    }
    {
        SkillValue skl{sklId, 22};
        auto sklStat = SkillStats::get_instance().get_dexterity_stats(skl);

        REQUIRE(sklStat.reaction_adjustment == 4);
        REQUIRE(sklStat.missile_attack_adjustment == 4);
        REQUIRE(sklStat.defensive_adjustment == -5);
    }
}

TEST_CASE("[TC-SKST.003] Constitution statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::constitution;
    {
        SkillValue skl{sklId, 6};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == -1);
        REQUIRE_FALSE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE_FALSE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.system_shock == 50);
        REQUIRE(sklStat.resurrection_survival == 55);
        REQUIRE(sklStat.poison_save == 0);
        REQUIRE_FALSE(sklStat.regeneration_points.has_value());
        REQUIRE_FALSE(sklStat.regeneration_turns.has_value());
    }
    {
        SkillValue skl{sklId, 15};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == 1);
        REQUIRE_FALSE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE_FALSE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.system_shock == 90);
        REQUIRE(sklStat.resurrection_survival == 94);
        REQUIRE(sklStat.poison_save == 0);
        REQUIRE_FALSE(sklStat.regeneration_points.has_value());
        REQUIRE_FALSE(sklStat.regeneration_turns.has_value());
    }
    {
        SkillValue skl{sklId, 18};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == 2);
        REQUIRE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE(sklStat.hit_point_adjustment_warriors.value() == 4);
        REQUIRE_FALSE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.system_shock == 99);
        REQUIRE(sklStat.resurrection_survival == 100);
        REQUIRE(sklStat.poison_save == 0);
        REQUIRE_FALSE(sklStat.regeneration_points.has_value());
        REQUIRE_FALSE(sklStat.regeneration_turns.has_value());
    }
    {
        SkillValue skl{sklId, 20};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == 2);
        REQUIRE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE(sklStat.hit_point_adjustment_warriors.value() == 5);
        REQUIRE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.rolls_upgrade.value() == 1);
        REQUIRE(sklStat.system_shock == 99);
        REQUIRE(sklStat.resurrection_survival == 100);
        REQUIRE(sklStat.poison_save == 1);
        REQUIRE(sklStat.regeneration_points.has_value());
        REQUIRE(sklStat.regeneration_points.value() == 1);
        REQUIRE(sklStat.regeneration_turns.has_value());
        REQUIRE(sklStat.regeneration_turns.value() == 6);
    }
    {
        SkillValue skl{sklId, 22};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == 2);
        REQUIRE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE(sklStat.hit_point_adjustment_warriors.value() == 6);
        REQUIRE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.rolls_upgrade.value() == 2);
        REQUIRE(sklStat.system_shock == 99);
        REQUIRE(sklStat.resurrection_survival == 100);
        REQUIRE(sklStat.poison_save == 2);
        REQUIRE(sklStat.regeneration_points.has_value());
        REQUIRE(sklStat.regeneration_points.value() == 1);
        REQUIRE(sklStat.regeneration_turns.has_value());
        REQUIRE(sklStat.regeneration_turns.value() == 4);
    }
    {
        SkillValue skl{sklId, 25};
        auto sklStat = SkillStats::get_instance().get_constitution_stats(skl);

        REQUIRE(sklStat.hit_point_adjustment == 2);
        REQUIRE(sklStat.hit_point_adjustment_warriors.has_value());
        REQUIRE(sklStat.hit_point_adjustment_warriors.value() == 7);
        REQUIRE(sklStat.rolls_upgrade.has_value());
        REQUIRE(sklStat.rolls_upgrade.value() == 3);
        REQUIRE(sklStat.system_shock == 100);
        REQUIRE(sklStat.resurrection_survival == 100);
        REQUIRE(sklStat.poison_save == 4);
        REQUIRE(sklStat.regeneration_points.has_value());
        REQUIRE(sklStat.regeneration_points.value() == 1);
        REQUIRE(sklStat.regeneration_turns.has_value());
        REQUIRE(sklStat.regeneration_turns.value() == 1);
    }
}

TEST_CASE("[TC-SKST.004] Intelligence statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::intelligence;
    {
        SkillValue skl{sklId, 1};
        auto sklStat = SkillStats::get_instance().get_intelligence_stats(skl);

        REQUIRE(sklStat.number_of_languages == 0);
        REQUIRE_FALSE(sklStat.spell_level.has_value());
        REQUIRE_FALSE(sklStat.chance_to_learn_spell.has_value());
        REQUIRE(sklStat.max_number_of_spells_per_level.has_value());
        REQUIRE(sklStat.max_number_of_spells_per_level.value() == 0);
        REQUIRE_FALSE(sklStat.illusion_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 8};
        auto sklStat = SkillStats::get_instance().get_intelligence_stats(skl);

        REQUIRE(sklStat.number_of_languages == 1);
        REQUIRE_FALSE(sklStat.spell_level.has_value());
        REQUIRE_FALSE(sklStat.chance_to_learn_spell.has_value());
        REQUIRE(sklStat.max_number_of_spells_per_level.has_value());
        REQUIRE(sklStat.max_number_of_spells_per_level.value() == 0);
        REQUIRE_FALSE(sklStat.illusion_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 13};
        auto sklStat = SkillStats::get_instance().get_intelligence_stats(skl);

        REQUIRE(sklStat.number_of_languages == 3);
        REQUIRE(sklStat.spell_level.has_value());
        REQUIRE(sklStat.spell_level.value() == 6);
        REQUIRE(sklStat.chance_to_learn_spell.has_value());
        REQUIRE(sklStat.chance_to_learn_spell.value() == 55);
        REQUIRE(sklStat.max_number_of_spells_per_level.has_value());
        REQUIRE(sklStat.max_number_of_spells_per_level.value() == 9);
        REQUIRE_FALSE(sklStat.illusion_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 20};
        auto sklStat = SkillStats::get_instance().get_intelligence_stats(skl);

        REQUIRE(sklStat.number_of_languages == 9);
        REQUIRE(sklStat.spell_level.has_value());
        REQUIRE(sklStat.spell_level.value() == 9);
        REQUIRE(sklStat.chance_to_learn_spell.has_value());
        REQUIRE(sklStat.chance_to_learn_spell.value() == 96);
        REQUIRE_FALSE(sklStat.max_number_of_spells_per_level.has_value());
        REQUIRE(sklStat.illusion_immunity.has_value());
        REQUIRE(sklStat.illusion_immunity.value() == 2);
    }
    {
        SkillValue skl{sklId, 25};
        auto sklStat = SkillStats::get_instance().get_intelligence_stats(skl);

        REQUIRE(sklStat.number_of_languages == 20);
        REQUIRE(sklStat.spell_level.has_value());
        REQUIRE(sklStat.spell_level.value() == 9);
        REQUIRE(sklStat.chance_to_learn_spell.has_value());
        REQUIRE(sklStat.chance_to_learn_spell.value() == 100);
        REQUIRE_FALSE(sklStat.max_number_of_spells_per_level.has_value());
        REQUIRE(sklStat.illusion_immunity.has_value());
        REQUIRE(sklStat.illusion_immunity.value() == 7);
    }
}

TEST_CASE("[TC-SKST.005] Wisdom statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::wisdom;
    {
        SkillValue skl{sklId, 2};
        auto sklStat = SkillStats::get_instance().get_wisdom_stats(skl);

        REQUIRE(sklStat.magical_defence_adjustment == -4);
        REQUIRE_FALSE(sklStat.bonus_spell_level_1.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_2.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_3.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_4.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_5.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_6.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_7.has_value());
        REQUIRE(sklStat.chance_of_spell_failure == 60);
        REQUIRE_FALSE(sklStat.spell_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 9};
        auto sklStat = SkillStats::get_instance().get_wisdom_stats(skl);

        REQUIRE(sklStat.magical_defence_adjustment == 0);
        REQUIRE(sklStat.bonus_spell_level_1.has_value());
        REQUIRE(sklStat.bonus_spell_level_1.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_2.has_value());
        REQUIRE(sklStat.bonus_spell_level_2.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_3.has_value());
        REQUIRE(sklStat.bonus_spell_level_3.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_4.has_value());
        REQUIRE(sklStat.bonus_spell_level_4.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_5.has_value());
        REQUIRE(sklStat.bonus_spell_level_5.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_6.has_value());
        REQUIRE(sklStat.bonus_spell_level_6.value() == 0);
        REQUIRE(sklStat.bonus_spell_level_7.has_value());
        REQUIRE(sklStat.bonus_spell_level_7.value() == 0);
        REQUIRE(sklStat.chance_of_spell_failure == 20);
        REQUIRE_FALSE(sklStat.spell_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 16};
        auto sklStat = SkillStats::get_instance().get_wisdom_stats(skl);

        REQUIRE(sklStat.magical_defence_adjustment == 2);
        REQUIRE(sklStat.bonus_spell_level_1.has_value());
        REQUIRE(sklStat.bonus_spell_level_1.value() == 2);
        REQUIRE(sklStat.bonus_spell_level_2.has_value());
        REQUIRE(sklStat.bonus_spell_level_2.value() == 2);
        REQUIRE_FALSE(sklStat.bonus_spell_level_3.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_4.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_5.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_6.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_7.has_value());
        REQUIRE(sklStat.chance_of_spell_failure == 0);
        REQUIRE_FALSE(sklStat.spell_immunity.has_value());
    }
    {
        SkillValue skl{sklId, 20};
        auto sklStat = SkillStats::get_instance().get_wisdom_stats(skl);

        REQUIRE(sklStat.magical_defence_adjustment == 4);
        REQUIRE(sklStat.bonus_spell_level_1.has_value());
        REQUIRE(sklStat.bonus_spell_level_1.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_2.has_value());
        REQUIRE(sklStat.bonus_spell_level_2.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_3.has_value());
        REQUIRE(sklStat.bonus_spell_level_3.value() == 2);
        REQUIRE(sklStat.bonus_spell_level_4.has_value());
        REQUIRE(sklStat.bonus_spell_level_4.value() == 2);
        REQUIRE_FALSE(sklStat.bonus_spell_level_5.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_6.has_value());
        REQUIRE_FALSE(sklStat.bonus_spell_level_7.has_value());
        REQUIRE(sklStat.chance_of_spell_failure == 0);
        REQUIRE(sklStat.spell_immunity.has_value());
        REQUIRE(sklStat.spell_immunity.value() == 1);
    }
    {
        SkillValue skl{sklId, 25};
        auto sklStat = SkillStats::get_instance().get_wisdom_stats(skl);

        REQUIRE(sklStat.magical_defence_adjustment == 4);
        REQUIRE(sklStat.bonus_spell_level_1.has_value());
        REQUIRE(sklStat.bonus_spell_level_1.value() == 4);
        REQUIRE(sklStat.bonus_spell_level_2.has_value());
        REQUIRE(sklStat.bonus_spell_level_2.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_3.has_value());
        REQUIRE(sklStat.bonus_spell_level_3.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_4.has_value());
        REQUIRE(sklStat.bonus_spell_level_4.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_5.has_value());
        REQUIRE(sklStat.bonus_spell_level_5.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_6.has_value());
        REQUIRE(sklStat.bonus_spell_level_6.value() == 3);
        REQUIRE(sklStat.bonus_spell_level_7.has_value());
        REQUIRE(sklStat.bonus_spell_level_7.value() == 1);
        REQUIRE(sklStat.chance_of_spell_failure == 0);
        REQUIRE(sklStat.spell_immunity.has_value());
        REQUIRE(sklStat.spell_immunity.value() == 1);
    }
}

TEST_CASE("[TC-SKST.006] Charisma statistics are retrieved according to the skill value", "[skills, statistics]" )
{
    Defs::skill sklId = Defs::skill::charisma;
    {
        SkillValue skl{sklId, 3};
        auto sklStat = SkillStats::get_instance().get_charisma_stats(skl);

        REQUIRE(sklStat.maximum_number_of_henchmen == 1);
        REQUIRE(sklStat.loyalty_base == -6);
        REQUIRE(sklStat.reaction_adjustment.has_value());
        REQUIRE(sklStat.reaction_adjustment.value() == -5);
    }
    {
        SkillValue skl{sklId, 7};
        auto sklStat = SkillStats::get_instance().get_charisma_stats(skl);

        REQUIRE(sklStat.maximum_number_of_henchmen == 3);
        REQUIRE(sklStat.loyalty_base == -2);
        REQUIRE(sklStat.reaction_adjustment.has_value());
        REQUIRE(sklStat.reaction_adjustment.value() == -1);
    }
    {
        SkillValue skl{sklId, 13};
        auto sklStat = SkillStats::get_instance().get_charisma_stats(skl);

        REQUIRE(sklStat.maximum_number_of_henchmen == 5);
        REQUIRE(sklStat.loyalty_base == 0);
        REQUIRE(sklStat.reaction_adjustment.has_value());
        REQUIRE(sklStat.reaction_adjustment.value() == 1);
    }
    {
        SkillValue skl{sklId, 18};
        auto sklStat = SkillStats::get_instance().get_charisma_stats(skl);

        REQUIRE(sklStat.maximum_number_of_henchmen == 15);
        REQUIRE(sklStat.loyalty_base == 8);
        REQUIRE(sklStat.reaction_adjustment.has_value());
        REQUIRE(sklStat.reaction_adjustment.value() == 7);
    }
    {
        SkillValue skl{sklId, 24};
        auto sklStat = SkillStats::get_instance().get_charisma_stats(skl);

        REQUIRE(sklStat.maximum_number_of_henchmen == 45);
        REQUIRE(sklStat.loyalty_base == 20);
        REQUIRE(sklStat.reaction_adjustment.has_value());
        REQUIRE(sklStat.reaction_adjustment.value() == 13);
    }
}