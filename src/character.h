#ifndef CHARACTER_H
#define CHARACTER_H

#include <iostream>
#include <map>
#include <string>
#include <optional>
#include <vector>
#include <set>

#include <defs.h>
#include <common_types.h>

#include <skills.h>
#include <experience.h>
#include <hit_points.h>
#include <thaco.h>
#include <money_bag.h>
#include <inventory.h>
#include <spell_book.h>
#include <holy_symbol.h>
#include <saving_throws.h>
#include <thaco.h>
#include <racial_stats.h>

namespace Adndtk
{
    class Character
    {
    public:
        Character(const std::string& name, const Defs::character_class& cls, const Defs::race& raceId,
            const Defs::moral_alignment& align, const Defs::sex& sexId,
            const std::optional<Defs::deity>& deityId = std::nullopt);

        const std::string& name() const { return _name; };
        inline const Defs::character_class& get_class() const { return _cls; };
        std::vector<Defs::character_class> get_classes() const;

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
        CharacterExperience experience() const;

        // Health
        HPChangeType wound(const HP& hps);
        HPChangeType cure(const HP& hps);
        HPChangeType raise();
        HPChangeType restore();
        inline HP hp() const { return _hp.current(); };
        inline HP total_hp() const { return _hp.total(); };
        inline bool is_alive() const { return hp() > 0; };
        double health() const { return static_cast<double>(hp()) / _hp.total(); };

        // Magic
        const SpellBook& spell_book() const;
        const HolySymbol& holy_symbol() const;
        bool is_spell_caster() const;

        bool cast_spell(const Defs::wizard_spell& spellId);
        bool learn_spell(const Defs::wizard_spell& spellId);
        bool memorise_spell(const Defs::wizard_spell& spellId);
        bool remove_spell(const Defs::wizard_spell& spellId);
        bool erase_spell(const Defs::wizard_spell& spellId);

        bool cast_spell(const Defs::priest_spell& spellId);
        bool memorise_spell(const Defs::priest_spell& spellId);
        bool remove_spell(const Defs::priest_spell& spellId);

        // Saving throws
        bool save_roll(const Defs::saving_throw& savId, const short& bonusMalus = 0) const;
        const SavingScore& save_score(const Defs::saving_throw& savId) const;

        // Combat
        Defs::attack_result try_hit(const AC& ac, const short& bonusMalus = 0) const;


    private:
        std::string                             _name;
        Defs::character_class                   _cls;
        Defs::race                              _race;
        Defs::sex                               _sex;
        Defs::moral_alignment                   _align;
        std::optional<Defs::deity>              _deity;
        std::map<Defs::skill, SkillValue>       _skills;
        Experience                              _xp;
        HitPoints                               _hp;
        Thaco                                   _thaco;
        MoneyBag                                _money;
        Inventory                               _inventory;
        std::vector<OnSkillValueChange>         _sklChgCbk;
        std::map<Defs::character_class, std::set<Defs::skill>>
                                                _primeRequisites;
        SpellBook                               _spellBook;
        HolySymbol                              _holySymbol;
        bool                                    _forwardEvent;
        RacialStats                             _racialStats;

        void on_change_xp(const Defs::character_class &cls, const XPChangeType &chgType,
                                          const XP &prevXP, const ExperienceLevel &prevLvl,
                                          const XP &newXP, const ExperienceLevel &newLvl);

        void on_change_hp(const HPChangeType &chgType, const HP &prevHP, const XP &newHP);
        void notify_all(const SkillValue& precValue, const SkillValue& currValue);

        bool verify_moral_alignment() const;
        bool verify_worshipped_deity() const;
    };
}

#endif // CHARACTER_H