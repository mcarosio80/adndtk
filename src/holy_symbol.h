#ifndef HOLY_SYMBOL_H
#define HOLY_SYMBOL_H

#include <defs.h>
#include <common_types.h>
#include <map>

namespace Adndtk
{
    class HolySymbol
    {
    public:
        HolySymbol(const Defs::character_class& cls);
        ~HolySymbol();

        void set_priest_level(const ExperienceLevel& newLevel);
        void set_priest_wisdom(const short& wisdomValue);
        bool pray_for_spell(const Defs::priest_spell& spellId);
        bool remove(const Defs::priest_spell& spellId);
        
    private:
        ExperienceLevel                                                 _casterLevel;
        Defs::character_class                                           _priestClass;
        short                                                           _wisdomScore;
        std::map<SpellLevel, std::map<Defs::priest_spell, short>>       _spells;

        static Adndtk::SpellLevel get_spell_level(const Defs::priest_spell& spellId);

        bool is_sphere_allowed(const Defs::priest_spell& spellId);
        bool is_level_available(const Defs::priest_spell& spellId);
        bool exists(const Defs::priest_spell& spellId);
        void enable_levels();
    };
}

#endif // HOLY_SYMBOL_H