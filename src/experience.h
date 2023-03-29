#ifndef EXPERIENCE_H
#define EXPERIENCE_H

#include <map>

#include "../generated/defs.h"
#include "../generated/config.h"

#include "advancement_table.h"

namespace Adndtk
{
    class Experience
    {
    public:
        Experience();
        Experience(const Defs::character_class& cls);

        Experience& operator+= (AdvancementTable::xp points);
        Experience& operator-= (AdvancementTable::xp points);
        Experience& add(const Defs::character_class& cls, const AdvancementTable::xp& xp);
        Experience& subtract(const Defs::character_class& cls, const AdvancementTable::xp& xp);
        const AdvancementTable::xp& xp(const Defs::character_class& cls) const;
        const AdvancementTable::level& level(const Defs::character_class& cls) const;
        const AdvancementTable::xp& xp() const;
        const AdvancementTable::level& level() const;
        
        friend std::ostream& operator<< (std::ostream& out, const Experience& exp)
        {
            size_t count = 0;
            for (auto& x : exp._xps)
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
        Defs::character_class                                       _cls;
        std::map<Defs::character_class, AdvancementTable::xp>       _xps;
        std::map<Defs::character_class, AdvancementTable::level>    _levels;
    };
}

#endif // EXPERIENCE_H