#ifndef COMMON_TYPES_H
#define COMMON_TYPES_H

#include <cstdint>
#include <functional>
#include "../generated/defs.h"

namespace Adndtk
{
    using ExperienceLevel = short;
    using XP = int64_t;
    using HP = int;
    using SavingScore = short;
    using THAC0 = short;

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
        select_character_class,
        select_character_class_type,
        select_skill_modifier,
        select_school_of_magic_skill_requisite,
        select_level_advancement,
        select_level_advancement_factor,
        select_class_limits,
        select_saving_throws,
        select_thaco
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

    using OnXPChange = std::function<void(const Defs::character_class &cls, const XPChangeType &chgType,
                                          const XP &prevXP, const ExperienceLevel &prevLvl,
                                          const XP &newXP, const ExperienceLevel &newLvl)>;

    using OnHPChange = std::function<void(const HPChangeType &chgType, const HP &prevHP, const XP &newHP)>;
}

#endif // COMMON_TYPES_H