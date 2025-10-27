#ifndef __ADNDTK__SRC__ADVANCEMENT_TABLE_H
#define __ADNDTK__SRC__ADVANCEMENT_TABLE_H


#include <defs.h>
#include <map>

#include <common_types.h>

namespace Adndtk
{
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

#endif // __ADNDTK__SRC__ADVANCEMENT_TABLE_H