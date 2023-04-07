#ifndef TURN_UNDEAD_H
#define TURN_UNDEAD_H

#include "../generated/defs.h"
#include <common_types.h>
#include <map>

namespace Adndtk
{
    class TurnUndead
    {
    public:
        static TurnUndead& get_instance();
        TurnUndead(TurnUndead const&) = delete;
        void operator=(TurnUndead const&) = delete;

        std::pair<Defs::turn_effect, std::optional<short>> get(const ExperienceLevel& lvl, const Defs::turnable& t) const;

        std::pair<Defs::turn_effect, bool> try_turn(const ExperienceLevel& lvl, const Defs::turnable& t, const short& bonusMalus = 0) const;

    private:
        TurnUndead();
        ~TurnUndead();

        void init();
		static bool                         _initialised;
        std::map<ExperienceLevel,
            std::map<Defs::turnable,
                std::pair<Defs::turn_effect, std::optional<short>>
            >
        >                                   _turnScores;
    };
}

#endif // TURN_UNDEAD_H