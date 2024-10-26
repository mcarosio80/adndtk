#ifndef PROFICIENCIES_H
#define PROFICIENCIES_H

#include <defs.h>
#include <common_types.h>

namespace Adndtk
{
    class ProficiencySlots
    {
    public:
        ProficiencySlots();
        ProficiencySlots(const Defs::character_class_type& clsType);
        ~ProficiencySlots();

        const int& get_weapon_initial_score() const { return _weaponInitialScore; }
        const int& get_weapon_num_levels() const { return _weaponNumLevels; }
        const int& get_penalty() const { return _penalty; }
        const int& get_non_weapon_initial_score() const { return _nonWeaponInitialScore; }
        const int& get_non_weapon_num_levels() const { return _nonWeaponNumLevels; }

        const int get_free_weapon_slots(const CharacterExperience& exp);
        const int get_used_weapon_slots();
        const int get_total_weapon_slots(const CharacterExperience& exp);
        const int get_free_non_weapon_slots(const CharacterExperience& exp);
        const int get_used_non_weapon_slots();
        const int get_total_non_weapon_slots(const CharacterExperience& exp);

    private:
        int _weaponInitialScore;
        int _weaponNumLevels;
        int _penalty;
        int _nonWeaponInitialScore;
        int _nonWeaponNumLevels;

        std::map<Defs::equipment, int>      _weaponslots;
    };
}

#endif // PROFICIENCIES_H