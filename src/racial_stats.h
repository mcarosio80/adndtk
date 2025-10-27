#ifndef __ADNDTK__SRC__RACIAL_STATS_H
#define __ADNDTK__SRC__RACIAL_STATS_H

#include <defs.h>
#include <tuple>
#include <common_types.h>

namespace Adndtk
{
    class RacialStats
    {
    public:
        RacialStats();
        RacialStats(const Defs::race& race, const Defs::sex& sex);
        ~RacialStats();

        Defs::aging_effects get_age_range() const;

        inline const short& current_age() const { return _currentAge; };
        inline const short& max_age() const { return _maxAge; };
        inline const short& height() const { return _height; };
        inline const short& weight() const { return _weight; };

        bool grow_old(const short& years);
        bool rejuvenate(const short& years);

        RacialStats& operator+=(const OnSkillValueModified& cbk)
        {
            _sklChgCbk.push_back(cbk);
            return (*this);
        }

    private:
        Defs::race              _race;
        Defs::sex               _sex;
        short	            _currentAge;

        short                _height;
        short                _weight;
        short                _middleAge;
	    short                _oldAge;
	    short                _venerableAge;

        short                _startingAge;
	    short                _maxAge;

        std::vector<OnSkillValueModified>
                                _sklChgCbk;

        std::map<Defs::skill, short> get_skill_modifiers(const Defs::aging_effects& ageRangeFrom, const Defs::aging_effects& ageRangeTo) const;
    };
}

#endif // __ADNDTK__SRC__RACIAL_STATS_H