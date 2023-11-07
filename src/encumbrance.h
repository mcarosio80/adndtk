#ifndef ENCUMBRANCE_H
#define ENCUMBRANCE_H

#include <defs.h>
#include <skills.h>

#include <map>

namespace Adndtk
{
    class Encumbrance
    {
    public:
        static Encumbrance& get_instance();
        Encumbrance(Encumbrance const&) = delete;
        void operator=(Encumbrance const&) = delete;

        const short& base_movement(const Defs::race& raceId) const;
        short movement_factor(const SkillValue& strengthValue, const Defs::race& raceId, const double& weightAllowed) const;
        inline const short get_base_movement(const Defs::race& raceId) const
        {
            return _baseMovFactors.at(raceId);
        }

    private:
        Encumbrance();
        ~Encumbrance();

        void load_base_movement_factors();
        void load_modified_movement_rate();

		static bool                         _initialised;
        std::map<SkillValue, std::map<double, std::pair<short, short>>>
                                            _movFactors;
        std::map<Defs::race, short>         _baseMovFactors;
    };
}

#endif // ENCUMBRANCE_H