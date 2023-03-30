#include "experience.h"
#include "adndtk.h"


Adndtk::Experience::Experience()
    : _cls{}
{
}

Adndtk::Experience::Experience(const Adndtk::Defs::character_class& cls)
    : _cls{cls}
{
    auto classes = Cyclopedia::get_instance().split(cls);
    for (auto& c : classes)
    {
        _xps[c] = 0;
        _levels[c] = 1;
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