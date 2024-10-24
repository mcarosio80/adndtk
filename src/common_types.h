#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <map>

#include <cstdint>
#include <functional>
#include <defs.h>
#include <skills.h>

namespace Adndtk
{
    using ExperienceLevel = short;
    using XP = int64_t;
    using HP = int;
    using SavingScore = short;
    using THAC0 = short;
    using AC = short;
    using SpellLevel = short;
    using ThievingScore = short;

    enum class SkillGenerationMethod
    {
        standard,     // 3d6 per each skill
        best_of_four, // 4d6, the least of them is discarded
        best_of_each  // 3d6 twice, the best is chosen
    };

    enum class Query
    {
        select_coin,
        select_skill_boundaries_default,
        select_skill_boundaries_class_type,
        select_skill_boundaries_class,
        select_skill_boundaries_race,
        select_character_classes,
        select_character_class,
        select_character_class_type,
        select_skill_modifier,
        select_school_of_magic_skill_requisite,
        select_level_advancement,
        select_level_advancement_factor,
        select_class_limits,
        select_saving_throws,
        select_thaco,
        select_coin_exchange_rates,
        select_equipment,
        select_thief_ability_base_scores,
        select_thieving_skill_racial_adjustments,
        select_thieving_skill_dexterity_adjustments,
        select_thieving_skill_armour_adjustments,
        select_thieving_skill_no_armour_adjustments,
        select_wizard_spell,
        select_wizard_spell_school,
        select_school_of_magic_access,
        select_school_of_magic_per_race,
        select_school_of_magic_per_class,
        select_wizard_spell_progression,
        select_priest_spell,
        select_priest_spell_sphere,
        select_sphere_access_per_class,
        select_priest_spell_progression,
        select_priest_spells_per_class_level,
        select_priest_spells_per_level_deity,
        select_paladin_spell_progression,
        select_ranger_spell_progression,
        select_race_base_movement,
        select_modified_movement_rate,
        select_starting_money,
        select_primary_skills,
        select_wizard_spells_by_level,
        select_bard_spell_progression,
        select_deities_by_moral_alignment,
        select_moral_alignments_by_class,
        select_worshipper_alignments,
        select_racial_stats,
        select_aging_effects,
        select_calendars,
        select_calendar,
        select_calendar_reckonings,
        select_calendar_months,
        select_calendar_special_days,
        select_roll_of_the_year,
        select_gems_by_type,
        select_gem,
        select_gem_type,
        select_object_of_art_by_probability,
        select_magical_item,
        select_magical_item_limitations,
        select_magical_item_type_by_probability,
        select_magical_item_by_type_and_probability,
        select_magical_items_by_type,
        select_treasure_components,
        select_treasure_composition,
        select_class_availability_per_race,
        select_race_availability_per_class,

        select_all_strength_stats,
        select_all_dexterity_stats,
        select_all_constitution_stats,
        select_all_intelligence_stats,
        select_all_wisdom_stats,
        select_all_charisma_stats,
        select_all_race,
        select_all_character_class,
        select_all_character_class_type,
        select_all_skill,
        select_all_deity,
        select_all_turn_undead,
        select_all_equipment,
        select_all_coin,
        select_all_moral_alignment,
        select_all_sex,
        select_all_saving_throw,
        select_all_cult,
        select_all_deity_rank,
    };

    enum class XPChangeType
    {
        none,
        level_up,
        level_down,
        level_zero,
        death
    };

    enum class HPChangeType
    {
        none,
        wounded,
        healed,
        dead
    };

    enum AddSpellResult
    {
        none,
        success,
        failure,
        existing,
        level_not_available,
        school_not_allowed,
        no_capacity
    };

    struct Const
    {
        static constexpr SpellLevel spell_book_limit = 9;
        static constexpr SpellLevel holy_symbol_limit = 7;
        static constexpr short high_value_for_prime_requisite = 16;
        static constexpr double xp_bonus_10 = 0.1;
        static constexpr double xp_bonus_none = 0.0;
        static constexpr SavingScore base_saving_throw = 20;
        static constexpr THAC0 base_thaco = 20;
        static constexpr THAC0 critical_hit = 20;
        static constexpr THAC0 critical_miss = 1;
        static constexpr double clothes_weight = 2.5;
        static constexpr short high_people_base_movement_factor = 12;
        static constexpr short short_people_base_movement_factor = 6;
        static constexpr short min_extended_skill_value = 1;
        static constexpr short min_skill_value = 3;
        static constexpr short max_skill_value = 18;
        static constexpr short max_extended_skill_value = 25;
    };

    using OnXPChange = std::function<void(const Defs::character_class &cls, const XPChangeType &chgType,
                                          const XP &prevXP, const ExperienceLevel &prevLvl,
                                          const XP &newXP, const ExperienceLevel &newLvl)>;

    using OnHPChange = std::function<void(const HPChangeType &chgType, const HP &prevHP, const XP &newHP)>;

    using OnSkillValueChange = std::function<void(const SkillValue& prevValue, const SkillValue& newValue)>;
    using OnSkillValueModified = std::function<void(const Defs::skill& skl, const short& sklModifier)>;

    class CharacterExperience
    {
        public:
            CharacterExperience(const Defs::character_class& cls);

            inline auto cbegin() const noexcept { return _xps.cbegin(); };
            inline auto cend() const noexcept { return _xps.cend(); };
            inline auto begin() noexcept { return _xps.begin(); };
            inline auto end() noexcept { return _xps.end(); };

            inline const ExperienceLevel& level(const Defs::character_class& cls) const { return _xps.at(cls).first; };
            inline const XP& xp(const Defs::character_class& cls) const { return _xps.at(cls).second; };
            void set(const Defs::character_class& cls, const ExperienceLevel& lvl, const XP& pts);
            void set_xp(const Defs::character_class& cls, const XP& pts);
            void set_level(const Defs::character_class& cls, const ExperienceLevel& lvl);

        private:
            std::map<Defs::character_class, std::pair<ExperienceLevel, XP>> _xps;
    };
}

#endif // COMMON_TYPES_H