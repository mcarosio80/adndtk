#ifndef __ADNDTK__SRC__SKILL_CREATOR_H
#define __ADNDTK__SRC__SKILL_CREATOR_H

#include <iostream>
#include <optional>
#include <tuple>

#include <defs.h>

#include <skills.h>
#include <common_types.h>
#include <queries.h>

namespace Adndtk
{
    class SkillCreator
    {
    public:
        SkillCreator();
        SkillCreator(const std::optional<Defs::character_class> &characterClass, const std::optional<Defs::race> &characterRace);
        SkillValue create(const Defs::skill &skillType, const SkillGenerationMethod &method = SkillGenerationMethod::standard);
        static SkillValue create(const Defs::skill &skillType, const std::optional<Defs::character_class>& cls,
                                const std::optional<Defs::race>& race, const SkillGenerationMethod &method = SkillGenerationMethod::standard);
        static void get_skill_constraints(const Query &queryId, const Defs::skill& skillType, const std::optional<int> &object, int &minValue, int &maxValue);
        static std::pair<int, int> get_skill_constraints(const Query &queryId, const Defs::skill& skillType, const std::optional<int> &object);
        static short get_race_adjustments(const Defs::skill &skillType, const Defs::race& race);

    private:
        static short generate_value(const SkillGenerationMethod &method = SkillGenerationMethod::standard);

        static std::pair<int, int> get_default_boundaries(const Defs::skill &skillType);
        static std::pair<int, int> get_race_boundaries(const Defs::skill &skillType, const Defs::race& race);
        static std::pair<int, int> get_class_type_boundaries(const Defs::skill &skillType, const Defs::character_class_type& clsType);
        static std::pair<int, int> get_class_boundaries(const Defs::skill &skillType, const Defs::character_class& cls);
        
        std::optional<Defs::character_class> _class;
        std::optional<Defs::race> _race;
        short _skillValue;
        short _excValue;
    };
}

#endif // __ADNDTK__SRC__SKILL_CREATOR_H