#ifndef HOLY_SYMBOL_H
#define HOLY_SYMBOL_H

#include <defs.h>
#include <common_types.h>
#include <skills.h>
#include <map>

namespace Adndtk
{
    class HolySymbol
    {
    public:
        HolySymbol(const Defs::character_class& cls);
        ~HolySymbol();

        const short& operator[] (Defs::priest_spell spellId) const;

        void set_priest_level(const ExperienceLevel& newLevel);
        void set_priest_wisdom(const short& wisdomValue);
        bool pray_for_spell(const Defs::priest_spell& spellId);
        bool remove(const Defs::priest_spell& spellId);

        short free_slots(const SpellLevel& spellLevel) const;
        short used_slots(const SpellLevel& spellLevel) const;
        short total_slots(const SpellLevel& spellLevel) const;

        const ExperienceLevel& caster_level() { return _casterLevel; }
        const Defs::character_class& caster_class() { return _casterClass; }
        const SkillValue& wisdom() { return _wisdomScore; }
        
    private:
        ExperienceLevel                                                 _casterLevel;
        Defs::character_class                                           _casterClass;
        SkillValue                                                      _wisdomScore;
        std::map<SpellLevel, std::map<Defs::priest_spell, short>>       _spells;

        static Adndtk::SpellLevel get_spell_level(const Defs::priest_spell& spellId);

        bool is_sphere_allowed(const Defs::priest_spell& spellId);
        //bool is_level_available(const Defs::priest_spell& spellId);
        bool exists(const Defs::priest_spell& spellId);
        void enable_levels();
        short get_bonus_spells(const SpellLevel& spellLevel) const;

        void fill_level(const SpellLevel& spellLevel);
        void erase_level(const SpellLevel& spellLevel);
    };
}

#endif // HOLY_SYMBOL_H