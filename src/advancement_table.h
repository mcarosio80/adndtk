#ifndef ADVANCEMENT_TABLE_H
#define ADVANCEMENT_TABLE_H


#include "../generated/defs.h"
#include <map>
#include <cstdint>

namespace Adndtk
{
    class AdvancementTable
    {
    public:
        using level = short;
        using xp = int64_t;
        
        AdvancementTable();

        level get_level(const Defs::character_class& cls, const xp& points) const;
        xp get_xp_for_level(const Defs::character_class& cls, const level& lvl) const;

        void set_advancement_factor(const Defs::character_class& cls, const xp& points);
        void add_level(const Defs::character_class& cls, const level& lvl, const xp& points);

    private:
        std::map<Defs::character_class, std::map<xp, level>>
                                                    _xpTable;
        std::map<Defs::character_class, std::map<level, xp>>
                                                    _lvlTable;
        std::map<Defs::character_class, xp>         _advancementFactor;
        std::map<Defs::character_class, level>      _titleLevel;
    };
}

#endif // EXPERIENCE_H