#ifndef THACO_H
#define THACO_H

#include <defs.h>
#include <common_types.h>

#include <map>
#include <set>

namespace Adndtk
{
    class Thaco
    {
    public:
        Thaco();
        Thaco(const Defs::character_class_type& clsType);
        ~Thaco();

        THAC0 get(const ExperienceLevel& lvl) const;
        Defs::attack_result try_hit(const Adndtk::CharacterExperience& levels, const AC& ac, const short& bonusMalus = 0) const;
        std::pair<Defs::character_class_type, ExperienceLevel> attack_as(const CharacterExperience& levels) const;
        
    private:
        std::set<Defs::character_class_type>                _clsType;
        std::map<Defs::character_class_type, THAC0>         _thacoScore;
        std::map<Defs::character_class_type, THAC0>         _thacoFactor;

        THAC0 get(const Defs::character_class_type& clsType, const ExperienceLevel& lvl) const;
    };
}

#endif // THACO_H