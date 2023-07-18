#ifndef THIEF_ABILITY_H
#define THIEF_ABILITY_H

#include <defs.h>
#include <common_types.h>
#include <map>
#include <vector>
#include <optional>

namespace Adndtk
{
    class ThiefAbility
    {
    public:
        ThiefAbility(const Defs::race& r);
        ~ThiefAbility();

        const Defs::race& race() const { return _race; }
        const ExperienceLevel& level() const { return _level; }
        const std::optional<Defs::equipment>& armour_in_use() const { return _armourInUse; }
        const short& dexterity_value() const { return _dexterityValue; }
        short available_ability_slots() const;

        ThievingScore operator[] (const Defs::thief_ability& abilityId) const;
        ThievingScore get(const Defs::thief_ability& abilityId, const short& bonusMalus) const;

        void armour_change(const std::optional<Defs::equipment>& armourId = std::nullopt);
        void dexterity_change(const short& skillValue);
        short level_change(const ExperienceLevel& newLevel);

        void improve_abilities(const std::vector<ThievingScore>& scores);

        void improve_abilities(const ThievingScore& pickPockets, const ThievingScore& openLocks, const ThievingScore& findRemoveTraps,
                            const ThievingScore& moveSilently, const ThievingScore& hideInShadows, const ThievingScore& hearNoise,
                            const ThievingScore& climbWalls, const ThievingScore& readLanguages);
        
    private:
        Defs::race                              _race;
        ExperienceLevel                         _level;
        ExperienceLevel                         _improvementsAvailable;
        std::optional<Defs::equipment>          _armourInUse;
        short                                   _dexterityValue;

        std::map<Defs::thief_ability, ThievingScore>    _baseAbilities;
        std::map<Defs::thief_ability, std::vector<ThievingScore>>
                                                        _experienceAdjustments;
        std::map<Defs::thief_ability, ThievingScore>    _raceModifiers;
        std::map<Defs::thief_ability, ThievingScore>    _armourModifiers;
        std::map<Defs::thief_ability, ThievingScore>    _dexterityModifiers;

        void set_race_abilities(const Defs::race& race);
        void set_armour_abilities(const std::optional<Defs::equipment>& armourId);
        void set_dexterity_abilities(const short& skillValue);
        void set_base_abilities();

        ThievingScore experience_adjustment(const Defs::thief_ability& abilityId) const;
        void increase_abilities(const std::vector<ThievingScore>& scores);
    };
}

#endif // THIEF_ABILITY_H