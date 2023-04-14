#ifndef SKILL_STATS_H
#define SKILL_STATS_H

#include <defs.h>
#include <tables.h>
#include <skills.h>
#include <map>

namespace Adndtk
{
    class SkillStats
    {
    public:
        static SkillStats& get_instance();
        SkillStats(SkillStats const&) = delete;
        void operator=(SkillStats const&) = delete;

        const Tables::strength_stats& get_strength_stats(const SkillValue& skillValue) const;
        const Tables::dexterity_stats& get_dexterity_stats(const SkillValue& skillValue) const;
        const Tables::constitution_stats& get_constitution_stats(const SkillValue& skillValue) const;
        const Tables::intelligence_stats& get_intelligence_stats(const SkillValue& skillValue) const;
        const Tables::wisdom_stats& get_wisdom_stats(const SkillValue& skillValue) const;
        const Tables::charisma_stats& get_charisma_stats(const SkillValue& skillValue) const;

    private:
        SkillStats();
        ~SkillStats();

        void init();
        void init_stats_for_strength();
        void init_stats_for_dexterity();
        void init_stats_for_constitution();
        void init_stats_for_intelligence();
        void init_stats_for_wisdom();
        void init_stats_for_charisma();
		static bool                         _initialised;
        
        std::map<SkillValue, Adndtk::Tables::strength_stats>        _strengthStats;
        std::map<SkillValue, Adndtk::Tables::dexterity_stats>       _dexterityStats;
        std::map<SkillValue, Adndtk::Tables::constitution_stats>    _constitutionStats;
        std::map<SkillValue, Adndtk::Tables::intelligence_stats>    _intelligenceStats;
        std::map<SkillValue, Adndtk::Tables::wisdom_stats>          _wisdomStats;
        std::map<SkillValue, Adndtk::Tables::charisma_stats>        _charismaStats;
    };
}

#endif // SKILL_STATS_H