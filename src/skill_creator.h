#ifndef SKILL_CREATOR_H
#define SKILL_CREATOR_H

#include <iostream>
#include <optional>

#include "../generated/defs.h"

#include <skills.h>
#include <query_result.h>

namespace Adndtk
{
    class SkillCreator
    {
    public:
        enum class Method
        {
            standard,     // 3d6 per each skill
            best_of_four, // 4d6, the least of them is discarded
            best_of_each  // 3d6 twice, the best is chosen
        };

        SkillCreator();
        SkillCreator(const std::optional<Defs::character_class> &characterClass, const std::optional<Defs::race> &characterRace);
        SkillValue create(const Defs::skill &skillType, const Method &method = Method::standard);

    private:
        short generate_value(const Method &method = Method::standard);
        void get_skill_constraints(const Query &queryId, const int &skillType, const std::optional<int> &object, int &minValue, int &maxValue);

        std::optional<Defs::character_class> _class;
        std::optional<Defs::race> _race;
        short _skillValue;
        short _excValue;
    };
}

#endif // SKILL_CREATOR_H