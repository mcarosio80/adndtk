#ifndef HIT_POINTS_H
#define HIT_POINTS_H

#include <iostream>
#include <map>
#include <vector>
#include <numeric>

#include "../generated/defs.h"

#include <advancement_table.h>

namespace Adndtk
{
    using HP = int;

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
                auto t = std::accumulate(x.second.begin(), x.second.begin() + _levels.at(x.first), 0);
                total += std::max(1, static_cast<int>(t / _hps.size()));
            }
            return total;
        }

        //HitPoints& shrink(const ExperienceLevel& count = 1);
        HitPoints& shrink(const Defs::character_class& cls, const ExperienceLevel& count = 1);
        //HitPoints& increase(const ExperienceLevel& count = 1);
        HitPoints& increase(const Defs::character_class& cls, const ExperienceLevel& count = 1);

        ExperienceLevel length();
        ExperienceLevel length(const Defs::character_class& cls);

    private:
        Defs::character_class                               _cls;
        std::map<Defs::character_class, std::vector<HP>>    _hps;
        std::map<Defs::character_class, ExperienceLevel>    _levels;
        std::map<Defs::character_class, Defs::die>          _hitDice;

        HP generate_hp(const Defs::character_class& cls);
        //Defs::character_class min_level();
        //Defs::character_class max_level();
    };
}

#endif // HIT_POINTS_H