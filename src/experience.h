#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <map>
#include <optional>

#include <defs.h>
#include <config.h>

#include <common_types.h>

namespace Adndtk
{
    class Experience
    {
    public:
        Experience();
        Experience(const Defs::character_class &cls, const Defs::race& race);

        Experience &operator+=(const OnXPChange &cbk);

        Experience& operator+=(const XP& points);
        Experience& operator-=(const XP& points);
        Experience& operator+=(const CharacterExperience& points);
        Experience& operator-=(const CharacterExperience& points);
        Experience& add(const ExperienceLevel& lvl);
        Experience& add(const Defs::character_class& cls, const ExperienceLevel& lvl);
        Experience& subtract(const ExperienceLevel& lvl);
        Experience& subtract(const Defs::character_class& cls, const ExperienceLevel& lvl);

        void set_xp_bonus(const Defs::character_class& cls, const double& xpBonus);

        XP &xp(const Defs::character_class &cls);
        ExperienceLevel &level(const Defs::character_class &cls);
        const XP &xp() const;
        const ExperienceLevel &level() const;

        ExperienceLevel& limit(const Defs::character_class& cls);

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
        Defs::character_class   _cls;
        Defs::race              _race;
        std::map<Defs::character_class, XP> _xps;
        std::map<Defs::character_class, ExperienceLevel> _levels;
        std::map<Defs::character_class, ExperienceLevel> _limits;
        std::vector<OnXPChange> _cbks;
        std::map<Defs::character_class, double> _xpBonus;

        Experience &set_xp(const Defs::character_class &cls, const XP &xp);
        Defs::character_class highest_level();
        void notify_all(const XP &prevXP, const ExperienceLevel &prevLvl,
                        const Defs::character_class &cls, const XPChangeType &chgType);

        XP adjust_xp(const Defs::character_class &cls, const XP& pts) const;
    };
}

#endif // EXPERIENCE_H