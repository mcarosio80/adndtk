#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <map>

#include "../generated/defs.h"
#include "../generated/config.h"

#include <common_types.h>

namespace Adndtk
{
    class Experience
    {
    public:
        Experience();
        Experience(const Defs::character_class &cls);

        Experience &operator+=(const OnXPChange &cbk);

        Experience &operator+=(XP points);
        Experience &operator-=(XP points);
        const XP &xp(const Defs::character_class &cls) const;
        const ExperienceLevel &level(const Defs::character_class &cls) const;
        const XP &xp() const;
        const ExperienceLevel &level() const;

        friend std::ostream &operator<<(std::ostream &out, const Experience &exp)
        {
            size_t count = 0;
            for (auto &x : exp._xps)
            {
                count++;
                if (count > 1)
                {
                    out << "/";
                }
                out << x.second;
            }
            return out;
        }

    private:
        Defs::character_class _cls;
        std::map<Defs::character_class, XP> _xps;
        std::map<Defs::character_class, ExperienceLevel> _levels;
        std::vector<OnXPChange> _cbks;

        Experience &set_xp(const Defs::character_class &cls, const XP &xp);
        void notify_all(const XP &prevXP, const ExperienceLevel &prevLvl,
                        const Defs::character_class &cls, const XPChangeType &chgType);
    };
}

#endif // EXPERIENCE_H