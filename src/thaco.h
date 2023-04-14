#ifndef THACO_H
#define THACO_H

#include <defs.h>
#include <common_types.h>

namespace Adndtk
{
    class Thaco
    {
    public:
        Thaco();
        Thaco(const Defs::character_class_type& clsType);
        ~Thaco();

        THAC0 get(const ExperienceLevel& lvl) const;
        Defs::attack_result try_hit(const Adndtk::ExperienceLevel& lvl, const AC& ac, const short& bonusMalus = 0) const;

    private:
        Defs::character_class_type  _clsType;
        short                       _thacoScore;
        short                       _thacoFactor;
    };
}

#endif // THACO_H