#ifndef CHARACTER_GENERATOR_H
#define CHARACTER_GENERATOR_H

#include <defs.h>
#include <tables.h>
#include <skills.h>

#include <vector>
#include <map>

namespace Adndtk
{
    class CharacterGenerator
    {
    public:
        CharacterGenerator();
        ~CharacterGenerator();

        static std::pair<int, int> get_skill_constraints_by_class_type(const Defs::character_class_type& clsType, const Defs::skill& skillId);
        static std::pair<int, int> get_skill_constraints_by_class(const Defs::character_class& clsId, const Defs::skill& skillId);

        static std::vector<Adndtk::Tables::race> available_races(const SkillValue& strength, const SkillValue& dexterity, const SkillValue& constitution, const SkillValue& intelligence, const SkillValue& wisdom, const SkillValue& charisma);
        static std::vector<Adndtk::Tables::race> available_races(const std::map<Defs::skill, SkillValue>& skillValues);
        static std::vector<Adndtk::Tables::character_class> available_classes(const SkillValue& strength, const SkillValue& dexterity, const SkillValue& constitution, const SkillValue& intelligence, const SkillValue& wisdom, const SkillValue& charisma, const Defs::race& raceId);
        static std::vector<Adndtk::Tables::character_class> available_classes(const std::map<Defs::skill, SkillValue>& skills, const Defs::race& raceId);
        static std::vector<Adndtk::Tables::moral_alignment> available_moral_alignments(const Defs::character_class& classId);
        static std::vector<Adndtk::Tables::sex> available_sex();
        static std::vector<Adndtk::Tables::deity> available_deities();
    };
}

#endif // CHARACTER_GENERATOR_H