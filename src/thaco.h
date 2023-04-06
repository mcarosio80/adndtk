#ifndef THACO_H
#define THACO_H

#include "../generated/defs.h"
#include <common_types.h>

namespace Adndtk
{
    class Thaco
    {
    public:
        Thaco();
        Thaco(const Defs::character_class_type& clsType);
        ~Thaco();

        THAC0 get(const ExperienceLevel& lvl);

    private:
        Defs::character_class_type  _clsType;
        short                       _thacoScore;
        short                       _thacoFactor;
    };
}

#endif // THACO_H