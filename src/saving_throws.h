#ifndef SAVING_THROWS_H
#define SAVING_THROWS_H

#include "../generated/defs.h"
#include <common_types.h>
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

    private:
        SavingThrows();
        ~SavingThrows();

        bool init();
		static bool                         _initialised;
        std::map<Defs::character_class_type,
            std::map<ExperienceLevel,
                std::map<Defs::saving_throw, SavingScore>
            >
        >                                   _savingScores;
    };
}

#endif // SAVING_THROWS_H