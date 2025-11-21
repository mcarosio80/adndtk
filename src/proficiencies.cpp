#include <proficiencies.h>
#include <cyclopedia.h>
#include <adnd_errors.h>

#include <cstdlib>

Adndtk::ProficiencySlots::ProficiencySlots(const Defs::character_class& clsId)
    : _clsId{clsId}, _weaponSlots{}, _weaponInitialScore{}, _weaponNumLevels{}, _penalty{},
        _nonWeaponInitialScore{}, _nonWeaponNumLevels{}, _nonWeaponSlots{}
{
    auto clsType = Cyclopedia::get_instance().get_class_type(clsId);
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

    if (_clsId == Defs::character_class::fighter)
    {
        _specialAttacks = Tables::specialist_attacks_per_round::to_map<int>("level");
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
    for (auto& s : _weaponSlots)
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
    return _nonWeaponSlots;
}

const int Adndtk::ProficiencySlots::get_total_non_weapon_slots(const CharacterExperience& exp)
{
    auto clsId = exp.get_higher_class();
    auto lvl = exp.level(clsId);

    return _nonWeaponInitialScore + std::div(lvl, _nonWeaponNumLevels).quot;
}

bool Adndtk::ProficiencySlots::is_proficient(const Defs::equipment& equipId) const
{
    return _weaponSlots.at(equipId) == Const::weapon_proficient_slots_num;
}

bool Adndtk::ProficiencySlots::is_specialised(const Defs::equipment& equipId) const
{
    return _clsId == Defs::character_class::fighter
        && _weaponSlots.at(equipId) == get_slots_for_specialisation(equipId);
}

Adndtk::Defs::weapon_proficiency_level Adndtk::ProficiencySlots::get_proficiency_level(const Defs::equipment& equipId) const
{
    if (is_proficient(equipId))
    {
        return Defs::weapon_proficiency_level::proficient;
    }
    else if (is_specialised(equipId))
    {
        return Defs::weapon_proficiency_level::specialised;
    }
    return Defs::weapon_proficiency_level::unexperienced;
}

Adndtk::Defs::weapon_proficiency_level Adndtk::ProficiencySlots::add_weapon_proficiency(const Defs::equipment& equipId)
{
    auto weapons = Tables::equipment::to_map<Defs::equipment>("id");

    auto itWpn = weapons.find(equipId);
    if (itWpn == weapons.end())
    {
        throw EquipmentException("Specified equipment does not exist");
    }
    if (itWpn->second.type != Defs::equipment_type::weapons)
    {
        throw EquipmentException("Specified equipment is not a weapon", itWpn->second.type);
    }

    const auto numSlotsForSpecialised = get_slots_for_specialisation(equipId);
    if (_weaponSlots.find(equipId) == _weaponSlots.end())
    {
        _weaponSlots[equipId] = 1;
    }
    else if (_clsId == Defs::character_class::fighter && _weaponSlots[equipId] < numSlotsForSpecialised)
    {
        _weaponSlots[equipId] += 1;
    }
    else
    {
        throw AdndException("Unable to further specialise");
    }

    return get_proficiency_level(equipId);
}

const int Adndtk::ProficiencySlots::get_modifier(const Defs::equipment& equipId) const
{
    if (_weaponSlots.find(equipId) == _weaponSlots.end())
    {
        return _penalty;
    }
    return 0;
}

const std::pair<short, short> Adndtk::ProficiencySlots::get_attack_rate(const Defs::equipment& equipId, const CharacterExperience& xp) const
{
    auto wpnInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_weapon_details, equipId);
    
    short numAttacks{1};
    short numRounds{1};
    if (is_specialised(equipId))
    {
        auto cls = xp.get_higher_class();
        auto lvl = xp.level(cls);

        if (wpnInfo.size() > 0)
        {
            auto& attackRates = _specialAttacks.lower_bound(lvl)->second;
            auto attType = wpnInfo[0].as<Defs::attack_type>("attack_type");
            if (attType == Defs::attack_type::melee)
            {
                numAttacks = attackRates.melee_weapon_attacks;
                numRounds = attackRates.melee_weapon_round;
            }
            else if (attType == Defs::attack_type::light_bow)
            {
                numAttacks = attackRates.light_x_bow_attacks;
                numRounds = attackRates.light_x_bow_round;
            }
            else if (attType == Defs::attack_type::heavy_bow)
            {
                numAttacks = attackRates.heavy_x_bow_attacks;
                numRounds = attackRates.heavy_x_bow_round;
            }
            else if (attType == Defs::attack_type::thrown_dagger)
            {
                numAttacks = attackRates.thrown_dagger_attacks;
                numRounds = attackRates.thrown_dagger_round;
            }
            else if (attType == Defs::attack_type::thrown_dart)
            {
                numAttacks = attackRates.thrown_dart_attacks;
                numRounds = attackRates.thrown_dart_round;
            }
            else if (attType == Defs::attack_type::missile)
            {
                numAttacks = attackRates.other_missile_attacks;
                numRounds = attackRates.other_missile_round;
            }
        }
    }
    return std::make_pair(numAttacks, numRounds);
}

bool Adndtk::ProficiencySlots::add_non_weapon_proficiency(const Defs::non_weapon_proficiency& profId, const CharacterExperience& xp)
{
    auto proficiencyIds = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(Query::select_non_weapon_proficiencies_by_class, _clsId).to_set<Defs::non_weapon_proficiency>("proficiency_id");

    if (proficiencyIds.find(profId) == proficiencyIds.end())
    {
        return false;
    }
    auto proficiencies = Tables::non_weapon_proficiency::to_map<Defs::non_weapon_proficiency>("id");
    auto& prof = proficiencies[profId];

    if (get_free_non_weapon_slots(xp) < prof.num_of_slots_required)
    {
        return false;
    }

    _nonWeaponSlots -= prof.num_of_slots_required;
    _nonWeaponProficiencies.emplace(profId);
    return true;
}

bool Adndtk::ProficiencySlots::is_proficient(const Defs::non_weapon_proficiency& profId) const
{
    return _nonWeaponProficiencies.find(profId) != _nonWeaponProficiencies.end();
}

short Adndtk::ProficiencySlots::get_slots_for_specialisation(const Defs::equipment& equipId) const
{
    auto wpnInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_weapon_details, equipId);
    auto wpnGroup = wpnInfo[0].as<Defs::weapon_group>("weapon_group");
    const auto numSlotsForSpecialised = (wpnGroup == Defs::weapon_group::crossbows)
                                        ? Const::x_bows_specialised_slots_num
                                        : Const::weapon_specialised_slots_num;

    return numSlotsForSpecialised;
}