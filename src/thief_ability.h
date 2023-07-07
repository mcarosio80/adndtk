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

        ThievingScore operator[] (const Defs::thief_ability& abilityId) const;

        void armour_change(const std::optional<Defs::equipment>& armourId = std::nullopt);
        void dexterity_change(const short& skillValue);

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
        std::map<Defs::thief_ability, ThievingScore>    _experienceAdjustments;
        std::map<Defs::thief_ability, ThievingScore>    _raceModifiers;
        std::map<Defs::thief_ability, ThievingScore>    _armourModifiers;
        std::map<Defs::thief_ability, ThievingScore>    _dexterityModifiers;

        void set_race_abilities(const Defs::race& race);
        void set_armour_abilities(const std::optional<Defs::equipment>& armourId);
        void set_dexterity_abilities(const short& skillValue);
        void set_base_abilities();
    };
}

#endif // THIEF_ABILITY_H