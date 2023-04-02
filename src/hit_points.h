#ifndef HIT_POINTS_H
#define HIT_POINTS_H

#include <iostream>
#include <map>
#include <vector>

#include "../generated/defs.h"

#include <common_types.h>

namespace Adndtk
{
    class HitPoints
    {
    public:
        HitPoints();
        HitPoints(const Defs::character_class& cls);

        friend std::ostream& operator<< (std::ostream& out, const HitPoints& hp)
        {
            HP total{hp};
            out << total;
            return out;
        }

        operator HP() const
        {
            HP total{0};
            for (auto& x : _hps)
            {
                for (int i=0; i<_levels.at(x.first); ++i)
                {
                    total += std::max(1, static_cast<int>(x.second[i] / _hps.size()));
                }
            }
            return total;
        }

        HitPoints& shrink(const Defs::character_class& cls, const ExperienceLevel& count = 1);
        HitPoints& increase(const Defs::character_class& cls, const ExperienceLevel& count = 1);

        ExperienceLevel length();
        ExperienceLevel length(const Defs::character_class& cls);

    private:
        Defs::character_class                               _cls;
        std::map<Defs::character_class, std::vector<HP>>    _hps;
        std::map<Defs::character_class, ExperienceLevel>    _levels;
        std::map<Defs::character_class, Defs::die>          _hitDice;

        HP generate_hp(const Defs::character_class& cls);
    };
}

#endif // HIT_POINTS_H