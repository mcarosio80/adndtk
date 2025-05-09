#include <advancement_table.h>


Adndtk::AdvancementTable::AdvancementTable()
{
}

Adndtk::ExperienceLevel Adndtk::AdvancementTable::get_level(const Adndtk::Defs::character_class& cls, const XP& points) const
{
    ExperienceLevel lvl{0};
    auto& table = _xpTable.at(cls);

    auto topXP = table.rbegin()->first;

    if (points < table.begin()->first)
    {
        lvl = 1;
    }
    else if (points > topXP)
    {
        lvl = table.rbegin()->second + (points - topXP) / _advancementFactor.at(cls);
    }
    else if (table.find(points) != table.end())
    {
        lvl = table.at(points);
    }
    else
    {
        auto it = table.upper_bound(points);
        lvl = it->second - 1;
    }
    
    return lvl;
}

Adndtk::XP Adndtk::AdvancementTable::get_xp_for_level(const Adndtk::Defs::character_class& cls, const ExperienceLevel& lvl) const
{
    XP pts{0};
    auto& table = _lvlTable.at(cls);

    if (lvl <= 1)
    {
        pts = 0;
    }
    else if (table.find(lvl) != table.end())
    {
        pts = table.at(lvl);
    }
    else
    {
        auto topMost = table.rbegin()->first;
        pts = (lvl - topMost) * _advancementFactor.at(cls) + table.rbegin()->second;
    }
    return pts;
}


void Adndtk::AdvancementTable::set_advancement_factor(const Defs::character_class& cls, const Adndtk::XP& points)
{
    _advancementFactor[cls] = points;

    // auto titLvl = _lvlTable[cls].rbegin()->first;
    // _titleLevel[cls] = titLvl;
}

void Adndtk::AdvancementTable::add_level(const Defs::character_class& cls, const Adndtk::ExperienceLevel& lvl, const Adndtk::XP& points)
{
    _xpTable[cls][points] = lvl;
    _lvlTable[cls][lvl] = points;
}