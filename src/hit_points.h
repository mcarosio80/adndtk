#ifndef __ADNDTK__SRC__HIT_POINTS_H
#define __ADNDTK__SRC__HIT_POINTS_H

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

        inline void set_constitution(const short& newVal)
        {
            update_constitution(SkillValue(Defs::skill::constitution, newVal), SkillValue(Defs::skill::constitution, newVal));
        }
        inline void update_constitution(const short& oldVal, const short& newVal)
        {
            update_constitution(SkillValue(Defs::skill::constitution, oldVal), SkillValue(Defs::skill::constitution, newVal));
        }
        void update_constitution(const SkillValue& oldVal, const SkillValue& newVal);

        operator HP() const;
        HitPoints& operator+=(const HP& hp);
        HitPoints& operator-=(const HP& hp);
        HitPoints& operator+=(const OnHPChange& cbk);
        HP total() const;
        HP current() const;

        HitPoints& shrink(const Defs::character_class& cls, const ExperienceLevel& count = 1);
        HitPoints& increase(const Defs::character_class& cls, const ExperienceLevel& count = 1);

        ExperienceLevel level() const;
        ExperienceLevel level(const Defs::character_class& cls) const;

    private:
        Defs::character_class                               _cls;
        std::map<Defs::character_class, std::vector<HP>>    _hps;
        std::map<Defs::character_class, ExperienceLevel>    _levels;
        std::map<Defs::character_class, Defs::die>          _hitDice;
        std::map<Defs::character_class, ExperienceLevel>    _titleLevel;
        std::map<Defs::character_class, HP>                 _hpAfterTitle;
        HP                                                  _currentHP;
        std::vector<OnHPChange>                             _cbks;
        std::optional<int>                                  _constitutionAdjustment;

        HP generate_hp(const Defs::character_class& cls, const ExperienceLevel& lvl);
        HP constitution_bonus() const;
        void notify_all_listeners(const HP& prevHP, const HP& newHP);
    };
}

#endif // __ADNDTK__SRC__HIT_POINTS_H