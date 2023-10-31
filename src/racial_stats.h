#ifndef RACIAL_STATS_H
#define RACIAL_STATS_H

#include <defs.h>
#include <cstdint>
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

        inline const uint16_t& current_age() const { return _currentAge; };
        inline const uint16_t& height() const { return _height; };
        inline const uint16_t& weight() const { return _weight; };

        bool grow_old(const uint16_t& years);
        bool rejuvenate(const uint16_t& years);

        RacialStats& operator+=(const OnSkillValueModified& cbk)
        {
            _sklChgCbk.push_back(cbk);
            return (*this);
        }

    private:
        Defs::race              _race;
        Defs::sex               _sex;
        uint16_t	            _currentAge;

        uint16_t                _height;
        uint16_t                _weight;
        uint16_t                _middleAge;
	    uint16_t                _oldAge;
	    uint16_t                _venerableAge;

        uint16_t                _startingAge;
	    uint16_t                _maxAge;

        std::vector<OnSkillValueModified>
                                _sklChgCbk;

        std::map<Defs::skill, short> get_skill_modifiers(const Defs::aging_effects& ageRangeFrom, const Defs::aging_effects& ageRangeTo) const;
    };
}

#endif // RACIAL_STATS_H