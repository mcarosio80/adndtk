#include <experience.h>
#include <cyclopedia.h>


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

Adndtk::Experience& Adndtk::Experience::operator+= (XP points)
{
    auto ptx = points / _xps.size();
    for (auto& x : _xps)
    {
        this->set_xp(x.first, ptx);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator-= (XP points)
{
    auto ptx = -points / _xps.size();
    for (auto& x : _xps)
    {
        this->set_xp(x.first, ptx);
    }
    return (*this);
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

const Adndtk::XP& Adndtk::Experience::xp(const Adndtk::Defs::character_class& cls) const
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _xps.at(cls);
}

const Adndtk::ExperienceLevel& Adndtk::Experience::level(const Adndtk::Defs::character_class& cls) const
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _levels.at(cls);
}

const Adndtk::XP& Adndtk::Experience::xp() const
{
    return xp(_cls);
}

const Adndtk::ExperienceLevel& Adndtk::Experience::level() const
{
    return level(_cls);
}

Adndtk::ExperienceLevel& Adndtk::Experience::limit(const Adndtk::Defs::character_class& cls)
{
    return _limits[cls];
}