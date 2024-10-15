#ifndef SKILL_CREATOR_H
#define SKILL_CREATOR_H

#include <iostream>
#include <optional>
#include <tuple>

#include <defs.h>

#include <skills.h>
#include <common_types.h>

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

    private:
        static short generate_value(const SkillGenerationMethod &method = SkillGenerationMethod::standard);
        
        std::optional<Defs::character_class> _class;
        std::optional<Defs::race> _race;
        short _skillValue;
        short _excValue;
    };
}

#endif // SKILL_CREATOR_H