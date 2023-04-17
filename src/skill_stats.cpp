#include <skill_stats.h>
#include <cyclopedia.h>


bool Adndtk::SkillStats::_initialised = false;



Adndtk::SkillStats::SkillStats()
{
}

Adndtk::SkillStats::~SkillStats()
{
}

Adndtk::SkillStats& Adndtk::SkillStats::get_instance()
{
    static SkillStats _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

const Adndtk::Tables::strength_stats& Adndtk::SkillStats::get_strength_stats(const SkillValue& skillValue) const
{
    return _strengthStats.upper_bound(skillValue)->second;
}

const Adndtk::Tables::dexterity_stats& Adndtk::SkillStats::get_dexterity_stats(const SkillValue& skillValue) const
{
    return _dexterityStats.at(skillValue);
}

const Adndtk::Tables::constitution_stats& Adndtk::SkillStats::get_constitution_stats(const SkillValue& skillValue) const
{
    return _constitutionStats.at(skillValue);
}

const Adndtk::Tables::intelligence_stats& Adndtk::SkillStats::get_intelligence_stats(const SkillValue& skillValue) const
{
    return _intelligenceStats.at(skillValue);
}

const Adndtk::Tables::wisdom_stats& Adndtk::SkillStats::get_wisdom_stats(const SkillValue& skillValue) const
{
    return _wisdomStats.at(skillValue);
}

const Adndtk::Tables::charisma_stats& Adndtk::SkillStats::get_charisma_stats(const SkillValue& skillValue) const
{
    return _charismaStats.at(skillValue);
}

void Adndtk::SkillStats::init()
{
    init_stats_for_strength();
    init_stats_for_dexterity();
    init_stats_for_constitution();
    init_stats_for_intelligence();
    init_stats_for_wisdom();
    init_stats_for_charisma();
}

void Adndtk::SkillStats::init_stats_for_strength()
{
    auto stats = Tables::strength_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::strength, s.ability_score_to, s.excp_to};
        _strengthStats[skl] = s;
    }
}

void Adndtk::SkillStats::init_stats_for_dexterity()
{
    auto stats = Tables::dexterity_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::dexterity, s.ability_score};
        _dexterityStats[skl] = s;
    }
}

void Adndtk::SkillStats::init_stats_for_constitution()
{
    auto stats = Tables::constitution_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::constitution, s.ability_score};
        _constitutionStats[skl] = s;
    }
}

void Adndtk::SkillStats::init_stats_for_intelligence()
{
    auto stats = Tables::intelligence_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::intelligence, s.ability_score};
        _intelligenceStats[skl] = s;
    }
}

void Adndtk::SkillStats::init_stats_for_wisdom()
{
    auto stats = Tables::wisdom_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::wisdom, s.ability_score};
        _wisdomStats[skl] = s;
    }
}

void Adndtk::SkillStats::init_stats_for_charisma()
{
    auto stats = Tables::charisma_stats::fetch_all();
    for (auto& s : stats)
    {
        SkillValue skl{Defs::skill::charisma, s.ability_score};
        _charismaStats[skl] = s;
    }
}