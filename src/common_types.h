#ifndef __ADNDTK__SRC__COMMON_TYPES_H
#define __ADNDTK__SRC__COMMON_TYPES_H

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
    using HitDice = double;

    enum class SkillGenerationMethod
    {
        standard,     // 3d6 per each skill
        best_of_four, // 4d6, the least of them is discarded
        best_of_each  // 3d6 twice, the best is chosen
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
        static constexpr THAC0 critical = 20;
        static constexpr THAC0 fumble = 1;
        static constexpr double clothes_weight = 2.5;
        static constexpr short high_people_base_movement_factor = 12;
        static constexpr short short_people_base_movement_factor = 6;
        static constexpr short min_extended_skill_value = 1;
        static constexpr short min_skill_value = 3;
        static constexpr short max_skill_value = 18;
        static constexpr short max_extended_skill_value = 25;
        static constexpr short weapon_proficient_slots_num = 1;
        static constexpr short weapon_specialised_slots_num = 2;
        static constexpr short x_bows_specialised_slots_num = 3;
        static constexpr AC max_ac_value = 20;
        static constexpr ExperienceLevel max_character_level = 20;
        static constexpr int spell_offset_level_1 = 1000;
        static constexpr double store_profit_margin_percentage{0.05};
        static constexpr uint32_t store_min_value_cp{1000};
        static constexpr uint32_t store_max_value_cp{10000};
        static constexpr uint32_t store_min_value_sp{1000};
        static constexpr uint32_t store_max_value_sp{5000};
        static constexpr uint32_t store_min_value_ep{1000};
        static constexpr uint32_t store_max_value_ep{2000};
        static constexpr uint32_t store_min_value_gp{100};
        static constexpr uint32_t store_max_value_gp{500};
        static constexpr uint32_t store_min_value_pp{100};
        static constexpr uint32_t store_max_value_pp{200};
        static constexpr uint32_t treasure_pool_min_value{1};
        static constexpr uint32_t treasure_pool_max_value{10000};
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

            Defs::character_class get_higher_class() const;
            Defs::character_class get_lower_class() const;

        private:
            std::map<Defs::character_class, std::pair<ExperienceLevel, XP>> _xps;
    };
}

#endif // __ADNDTK__SRC__COMMON_TYPES_H