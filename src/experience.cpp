#include <experience.h>
#include <cyclopedia.h>
#include <options.h>

#include <cmath>

Adndtk::Experience::Experience()
    : _cls{}, _race{}
{
}

Adndtk::Experience::Experience(const Adndtk::Defs::character_class& cls, const Defs::race& race)
    : _cls{cls}, _race{race}
{
    auto classes = Cyclopedia::get_instance().split(cls);
    for (auto& c : classes)
    {
        _xps[c] = 0;
        _levels[c] = 1;
        _xpBonus[c] = Const::xp_bonus_none;
    }

    int clsId = static_cast<int>(cls);
    int raceId = static_cast<int>(race);
    auto limits = Cyclopedia::get_instance().exec_prepared_statement<int, int>(Query::select_class_limits, clsId, raceId);
    for (auto& l : limits)
    {
        auto classId = static_cast<Defs::character_class>(l.as<int>("class_id"));
        auto lvlLimit = static_cast<ExperienceLevel>(l.as<int>("level_limit"));
        _limits[classId] = lvlLimit;
    }
}

Adndtk::Experience& Adndtk::Experience::operator+= (const Adndtk::OnXPChange& cbk)
{
    _cbks.push_back(cbk);
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator+= (const XP& points)
{
    auto ptx = points / _xps.size();
    for (auto& x : _xps)
    {
        auto adjPoints = adjust_xp(x.first, ptx);
        this->set_xp(x.first, adjPoints);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator-= (const XP& points)
{
    auto ptx = points / _xps.size();
    for (auto& x : _xps)
    {
        this->set_xp(x.first, -ptx);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator+= (const CharacterExperience& points)
{
    for (auto pt = points.cbegin(); pt != points.cend(); ++pt)
    {
        auto adjPoints = adjust_xp(pt->first, pt->second.second);
        this->set_xp(pt->first, adjPoints);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator-= (const CharacterExperience& points)
{
    for (auto pt = points.cbegin(); pt != points.cend(); ++pt)
    {
        this->set_xp(pt->first, -pt->second.second);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::add(const ExperienceLevel& lvl)
{
    auto& advTable = Cyclopedia::get_instance().advancement_table();
    for (ExperienceLevel l = 0; l<lvl; ++l)
    {
        XP minPts{0};
        auto chosenClass = _levels.cbegin()->first;
        for (auto& l : _levels)
        {
            auto pts = advTable.get_xp_for_level(l.first, l.second + 1) - _xps[l.first];
            if (minPts == 0 || pts < minPts)
            {
                minPts = pts;
                chosenClass = l.first;
            }
        }

        set_xp(chosenClass, minPts);
    }

    return (*this);
}

Adndtk::Experience& Adndtk::Experience::add(const Defs::character_class& cls, const ExperienceLevel& lvl)
{
    auto& advTable = Cyclopedia::get_instance().advancement_table();
    for (ExperienceLevel l = 0; l<lvl; ++l)
    {
        auto pts = advTable.get_xp_for_level(cls, _levels[cls] + 1) - _xps[cls];
        set_xp(cls, pts);
    }

    return (*this);
}

Adndtk::Experience& Adndtk::Experience::subtract(const ExperienceLevel& lvl)
{
    auto& advTable = Cyclopedia::get_instance().advancement_table();
    for (ExperienceLevel l = 0; l<lvl; ++l)
    {
        auto selectedClass = highest_level();
        auto selectedLevel = _levels[selectedClass];
        for (const auto& l : _levels)
        {
            if (l.second == selectedLevel)
            {
                XP xpRequired = advTable.get_xp_for_level(l.first, _levels[l.first] + 1) - _xps[l.first];
                XP xpHighest = advTable.get_xp_for_level(selectedClass, _levels[selectedClass] + 1) - _xps[selectedClass];
                if (xpRequired > xpHighest)
                {
                    selectedClass = l.first;
                    selectedLevel = l.second;
                }
            }
        }
        auto ptsPrec = advTable.get_xp_for_level(selectedClass, _levels[selectedClass] - 1);
        auto ptsCurr = advTable.get_xp_for_level(selectedClass, _levels[selectedClass]);
        auto pts = (ptsCurr + ptsPrec) / 2 + (_xps[selectedClass] - ptsCurr);
        set_xp(selectedClass, -pts);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::subtract(const Defs::character_class& cls, const ExperienceLevel& lvl)
{
    if (_levels[cls] < lvl)
    {
        notify_all(_xps[cls], _levels[cls], cls, XPChangeType::death);
    }
    else if (_levels[cls] == lvl)
    {
        notify_all(_xps[cls], _levels[cls], cls, XPChangeType::level_zero);
    }
    else
    {
        auto& advTable = Cyclopedia::get_instance().advancement_table();
        for (ExperienceLevel l = 0; l<lvl; ++l)
        {
            auto ptsPrec = advTable.get_xp_for_level(cls, _levels[cls] - 1);
            auto ptsCurr = advTable.get_xp_for_level(cls, _levels[cls]);
            auto pts = (_xps[cls] - ptsCurr) + (ptsCurr - (ptsCurr + ptsPrec) / 2);
            set_xp(cls, -pts);
        }
    }
    return (*this);
}

void Adndtk::Experience::set_xp_bonus(const Defs::character_class& cls, const double& xpBonus)
{
    _xpBonus[cls] = xpBonus;
}

Adndtk::Defs::character_class Adndtk::Experience::highest_level()
{
    auto cls = _levels.cbegin()->first;

    for (const auto& l : _levels)
    {
        if (l.second > _levels[cls])
        {
            cls = l.first;
        }
    }
    return cls;
}

void Adndtk::Experience::notify_all(const Adndtk::XP& prevXP, const Adndtk::ExperienceLevel& prevLvl,
                                            const Adndtk::Defs::character_class& cls, const Adndtk::XPChangeType& chgType)
{
    for (auto& cbk : _cbks)
    {
        cbk(cls, chgType, prevXP, prevLvl, _xps[cls], _levels[cls]);
    }
}

Adndtk::Experience& Adndtk::Experience::set_xp(const Adndtk::Defs::character_class& cls, const XP& xp)
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
        return *this;
    }

    if (xp == 0)
    {
        return *this;
    }

    auto precXP = _xps[cls];
    auto precLvl = _levels[cls];

    if (precLvl == 0 && xp < 0)
    {
        notify_all(precXP, precLvl, cls, XPChangeType::death);
    }

    if (xp < 0 && _xps[cls] < std::llabs(xp))
    {
        _xps[cls] = 0;
        _levels[cls] = 0;
        notify_all(precXP, precLvl, cls, XPChangeType::level_zero);
        return *this;
    }

    auto advTable = Cyclopedia::get_instance().advancement_table();
    ExperienceLevel currentLevel = advTable.get_level(cls, _xps[cls]);

    XP newXP = _xps[cls] + xp;
    ExperienceLevel newLevel = advTable.get_level(cls, newXP);
    if (_limits.find(cls) != _limits.end() && newLevel > _limits[cls])
    {
        newLevel = _limits[cls];
    }

    _xps[cls] = newXP;
    _levels[cls] = newLevel;

    if (currentLevel < newLevel)
    {
        notify_all(precXP, precLvl, cls, XPChangeType::level_up);
    }
    else if (currentLevel > newLevel)
    {
        notify_all(precXP, precLvl, cls, XPChangeType::level_down);
    }
    return (*this);
}

Adndtk::XP& Adndtk::Experience::xp(const Adndtk::Defs::character_class& cls)
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _xps[cls];
}

Adndtk::ExperienceLevel& Adndtk::Experience::level(const Adndtk::Defs::character_class& cls)
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _levels[cls];
}

const Adndtk::XP& Adndtk::Experience::xp() const
{
    return _xps.cbegin()->second;
}

const Adndtk::ExperienceLevel& Adndtk::Experience::level() const
{
    return _levels.cbegin()->second;
}

Adndtk::ExperienceLevel& Adndtk::Experience::limit(const Adndtk::Defs::character_class& cls)
{
    return _limits[cls];
}

Adndtk::XP Adndtk::Experience::adjust_xp(const Adndtk::Defs::character_class& cls, const XP& pts) const
{
    if (OptionalRules::get_instance().option<bool>(Option::enable_bonus_xp_for_high_prime_requisites))
    {
        return pts + static_cast<XP>(std::lround(pts * _xpBonus.at(cls)));
    }
    return pts;
}