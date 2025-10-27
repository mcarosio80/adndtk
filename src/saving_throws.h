#ifndef __ADNDTK__SRC__SAVING_THROWS_H
#define __ADNDTK__SRC__SAVING_THROWS_H

#include <defs.h>
#include <common_types.h>
#include <experience.h>

#include <map>

namespace Adndtk
{
    class SavingThrows
    {
    public:
        static SavingThrows& get_instance();
        SavingThrows(SavingThrows const&) = delete;
        void operator=(SavingThrows const&) = delete;

        const SavingScore& get_score(const Defs::character_class_type& type, const ExperienceLevel& lvl, const Defs::saving_throw& sav) const;
        const SavingScore& get_score(const Defs::character_class& cls, const CharacterExperience& levels, const Defs::saving_throw& sav) const;
        bool roll(const Defs::character_class_type& type, const ExperienceLevel& lvl, const Defs::saving_throw& sav, const short& bonusMalus) const;
        bool roll(const Defs::character_class& clsId, const CharacterExperience& levels, const Defs::saving_throw& sav, const short& bonusMalus) const;
        std::pair<Defs::character_class_type, ExperienceLevel> save_as(const Defs::character_class& clsId, const CharacterExperience& levels, const Defs::saving_throw& sav) const;

    private:
        SavingThrows();
        ~SavingThrows();

        void init();
		static bool                         _initialised;
        std::map<Defs::character_class_type,
            std::map<ExperienceLevel,
                std::map<Defs::saving_throw, SavingScore>
            >
        >                                   _savingScores;
    };
}

#endif // __ADNDTK__SRC__SAVING_THROWS_H