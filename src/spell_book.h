#ifndef SPELL_BOOK_H
#define SPELL_BOOK_H

#include <defs.h>
#include <common_types.h>
#include <skills.h>
#include <map>

namespace Adndtk
{
    class SpellBook
    {
    public:
        SpellBook(const Defs::character_class& cls);
        ~SpellBook();

        void set_wizard_level(const ExperienceLevel& newLevel);
        void set_wizard_intelligence(const short& intelligenceValue);
        AddSpellResult scribe_scroll(const Defs::wizard_spell& spellId);
        bool memorise(const Defs::wizard_spell& spellId);
        bool remove(const Defs::wizard_spell& spellId);
        bool delete_from_book(const Defs::wizard_spell& spellId);

        const ExperienceLevel& caster_level() { return _casterLevel; }
        const Defs::character_class& caster_class() { return _casterClass; }
        const SkillValue& intelligence() { return _intelligenceScore; }
        
    private:
        ExperienceLevel                                                 _casterLevel;
        Defs::character_class                                           _casterClass;
        SkillValue                                                      _intelligenceScore;
        std::map<SpellLevel, std::map<Defs::wizard_spell, short>>       _spells;

        static Adndtk::SpellLevel get_spell_level(const Defs::wizard_spell& spellId);

        bool has_capacity(const Defs::wizard_spell& spellId);
        bool is_school_allowed(const Defs::wizard_spell& spellId);
        bool is_level_available(const Defs::wizard_spell& spellId);
        bool exists(const Defs::wizard_spell& spellId);
        bool try_scribe(const Defs::wizard_spell& spellId);
    };
}

#endif // SPELL_BOOK_H