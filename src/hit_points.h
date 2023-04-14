#ifndef HIT_POINTS_H
#define HIT_POINTS_H

#include <iostream>
#include <map>
#include <vector>

#include <defs.h>

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
            out << hp.total();
            return out;
        }

        operator HP() const;
        HitPoints& operator+=(const HP& hp);
        HitPoints& operator-=(const HP& hp);
        HitPoints& operator+=(const OnHPChange& cbk);
        HP total() const;
        const HP& current() const;

        HitPoints& shrink(const Defs::character_class& cls, const ExperienceLevel& count = 1);
        HitPoints& increase(const Defs::character_class& cls, const ExperienceLevel& count = 1);

        ExperienceLevel level();
        ExperienceLevel level(const Defs::character_class& cls);

    private:
        Defs::character_class                               _cls;
        std::map<Defs::character_class, std::vector<HP>>    _hps;
        std::map<Defs::character_class, ExperienceLevel>    _levels;
        std::map<Defs::character_class, Defs::die>          _hitDice;
        HP                                                  _currentHP;
        std::vector<OnHPChange>                             _cbks;

        HP generate_hp(const Defs::character_class& cls);
        void notify_all_listeners(const HP& prevHP, const HP& newHP);
    };
}

#endif // HIT_POINTS_H