#ifndef PROFICIENCIES_H
#define PROFICIENCIES_H

#include <defs.h>
#include <common_types.h>
#include <tables.h>

namespace Adndtk
{
    class ProficiencySlots
    {
    public:
        ProficiencySlots(const Defs::character_class& clsId);
        ~ProficiencySlots();

        const int& get_base_weapon_initial_score() const { return _weaponInitialScore; }
        const int& get_base_weapon_num_levels() const { return _weaponNumLevels; }
        const int& get_base_penalty() const { return _penalty; }
        const int& get_base_non_weapon_initial_score() const { return _nonWeaponInitialScore; }
        const int& get_base_non_weapon_num_levels() const { return _nonWeaponNumLevels; }

        const int get_free_weapon_slots(const CharacterExperience& exp);
        const int get_used_weapon_slots();
        const int get_total_weapon_slots(const CharacterExperience& exp);
        const int get_free_non_weapon_slots(const CharacterExperience& exp);
        const int get_used_non_weapon_slots();
        const int get_total_non_weapon_slots(const CharacterExperience& exp);

        bool is_proficient(const Defs::equipment& equipId) const;
        bool is_specialised(const Defs::equipment& equipId) const;
        Defs::weapon_proficiency_level get_proficiency_level(const Defs::equipment& equipId) const;
        Defs::weapon_proficiency_level add_weapon_proficiency(const Defs::equipment& equipId);
        const int get_modifier(const Defs::equipment& equipId) const;
        const std::pair<short, short> get_attack_rate(const Defs::equipment& equipId, const CharacterExperience& xp) const;

        bool add_non_weapon_proficiency(const Defs::non_weapon_proficiency& profId, const CharacterExperience& xp);
        bool is_proficient(const Defs::non_weapon_proficiency& profId) const;

    private:
        int _weaponInitialScore;
        int _weaponNumLevels;
        int _penalty;
        int _nonWeaponInitialScore;
        int _nonWeaponNumLevels;

        short get_slots_for_specialisation(const Defs::equipment& equipId) const;

        std::map<Defs::equipment, int>                          _weaponSlots;
        int                                                     _nonWeaponSlots;
        std::set<Defs::non_weapon_proficiency>                  _nonWeaponProficiencies;
        Defs::character_class                                   _clsId;
        std::map<int, Tables::specialist_attacks_per_round>     _specialAttacks;
    };
}

#endif // PROFICIENCIES_H