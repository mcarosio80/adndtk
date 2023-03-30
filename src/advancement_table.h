#ifndef ADVANCEMENT_TABLE_H
#define ADVANCEMENT_TABLE_H


#include "../generated/defs.h"
#include <map>
#include <cstdint>

namespace Adndtk
{
    using ExperienceLevel = short;
    using XP = int64_t;

    class AdvancementTable
    {
    public:
        AdvancementTable();

        ExperienceLevel get_level(const Defs::character_class& cls, const XP& points) const;
        XP get_xp_for_level(const Defs::character_class& cls, const ExperienceLevel& lvl) const;

        void set_advancement_factor(const Defs::character_class& cls, const XP& points);
        void add_level(const Defs::character_class& cls, const ExperienceLevel& lvl, const XP& points);

    private:
        std::map<Defs::character_class, std::map<XP, ExperienceLevel>>
                                                            _xpTable;
        std::map<Defs::character_class, std::map<ExperienceLevel, XP>>
                                                            _lvlTable;
        std::map<Defs::character_class, XP>                 _advancementFactor;
        std::map<Defs::character_class, ExperienceLevel>    _titleLevel;
    };
}

#endif // EXPERIENCE_H