#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <map>
#include <string>
#include <optional>
#include <vector>

#include <defs.h>
#include <common_types.h>

#include <skills.h>
#include <experience.h>
#include <hit_points.h>
#include <thaco.h>
#include <money_bag.h>
#include <inventory.h>

namespace Adndtk
{
    class Character
    {
    public:
        Character(const std::string& name, const Defs::character_class& cls, const Defs::race& raceId,
            const Defs::sex& sexId, const std::optional<Defs::deity>& deityId = std::nullopt);

        const std::string& name() const { return _name; };
        std::vector<Defs::character_class> get_class() const;

        Character& operator+=(const OnSkillValueChange& cbk);

        // Skills
        const SkillValue& strength() const { return _skills.at(Defs::skill::strength); };
        const SkillValue& dexterity() const { return _skills.at(Defs::skill::dexterity); };
        const SkillValue& constitution() const { return _skills.at(Defs::skill::constitution); };
        const SkillValue& intelligence() const { return _skills.at(Defs::skill::intelligence); };
        const SkillValue& wisdom() const { return _skills.at(Defs::skill::wisdom); };
        const SkillValue& charisma() const { return _skills.at(Defs::skill::charisma); };
        void change_skill(const SkillValue& newValue);

        // Experience
        void gain_xp(const XP& xpValue, const std::optional<Defs::character_class>& cls = std::nullopt);
        void lose_xp(const XP& xpValue, const std::optional<Defs::character_class>& cls = std::nullopt);
        void gain_level(const ExperienceLevel& lvl, const std::optional<Defs::character_class>& cls = std::nullopt);
        void lose_level(const ExperienceLevel& lvl, const std::optional<Defs::character_class>& cls = std::nullopt);
        CharacterExperience experience();

        // Health
        HPChangeType wound(const HP& hps);
        HPChangeType cure(const HP& hps);
        HPChangeType raise();
        HPChangeType restore();
        inline HP hp() const { return _hp.current(); };
        inline HP total_hp() const { return _hp.total(); };
        inline bool is_alive() const { return hp() > 0; };
        double health() const { return static_cast<double>(hp()) / _hp.total(); };

    private:
        std::string                             _name;
        Defs::character_class                   _cls;
        Defs::race                              _race;
        Defs::sex                               _sex;
        std::optional<Defs::deity>              _deity;
        std::map<Defs::skill, SkillValue>       _skills;
        Experience                              _xp;
        HitPoints                               _hp;
        Thaco                                   _thaco;
        MoneyBag                                _money;
        Inventory                               _inventory;
        std::vector<OnSkillValueChange>         _sklChgCbk;

        bool                                    _forwardEvent;
        void on_change_xp(const Defs::character_class &cls, const XPChangeType &chgType,
                                          const XP &prevXP, const ExperienceLevel &prevLvl,
                                          const XP &newXP, const ExperienceLevel &newLvl);

        void on_change_hp(const HPChangeType &chgType, const HP &prevHP, const XP &newHP);
        void notify_all(const SkillValue& precValue, const SkillValue& currValue);
    };
}

#endif // CHARACTER_H