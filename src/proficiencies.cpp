#include <proficiencies.h>
#include <cyclopedia.h>

#include <cstdlib>

Adndtk::ProficiencySlots::ProficiencySlots()
{
}

Adndtk::ProficiencySlots::ProficiencySlots(const Defs::character_class_type& clsType)
{
    for (auto& clsTypeId : Cyclopedia::get_instance().split<Defs::character_class_type>(clsType))
    {
        auto resSet = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class_type>(Query::select_proficiency_slots_by_class_type, clsTypeId);
        for (auto& r : resSet)
        {
            auto weaponInitialScore = r.as<int>("weapon_initial_score");
            auto weaponNumLevels = r.as<int>("weapon_num_levels");
            auto penalty = r.as<int>("penalty");
            auto nonWeaponInitialScore = r.as<int>("non_weapon_initial_score");
            auto nonWeaponNumLevels = r.as<int>("non_weapon_num_levels");

            if (weaponInitialScore > _weaponInitialScore)
            {
                _weaponInitialScore = weaponInitialScore;
                _weaponNumLevels = weaponNumLevels;
                _penalty = penalty;
            }

            if (nonWeaponInitialScore > _nonWeaponInitialScore)
            {
                _nonWeaponInitialScore = nonWeaponInitialScore;
                _nonWeaponNumLevels = nonWeaponNumLevels;
            }
        }
    }
}

Adndtk::ProficiencySlots::~ProficiencySlots()
{
}

const int Adndtk::ProficiencySlots::get_free_weapon_slots(const CharacterExperience& exp)
{
    return get_total_weapon_slots(exp) - get_used_weapon_slots();
}

const int Adndtk::ProficiencySlots::get_used_weapon_slots()
{
    int total{0};
    for (auto& s : _weaponslots)
    {
        total += s.second;
    }
    return total;
}

const int Adndtk::ProficiencySlots::get_total_weapon_slots(const CharacterExperience& exp)
{
    auto clsId = exp.get_higher_class();
    auto lvl = exp.level(clsId);

    return _weaponInitialScore + std::div(lvl, _weaponNumLevels).quot;
}

const int Adndtk::ProficiencySlots::get_free_non_weapon_slots(const CharacterExperience& exp)
{
    return get_total_non_weapon_slots(exp) - get_used_non_weapon_slots();
}

const int Adndtk::ProficiencySlots::get_used_non_weapon_slots()
{
    return 0;
}

const int Adndtk::ProficiencySlots::get_total_non_weapon_slots(const CharacterExperience& exp)
{
    auto clsId = exp.get_higher_class();
    auto lvl = exp.level(clsId);

    return _nonWeaponInitialScore + std::div(lvl, _nonWeaponNumLevels).quot;
}