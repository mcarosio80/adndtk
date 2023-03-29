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

Adndtk::Experience& Adndtk::Experience::operator+= (AdvancementTable::xp points)
{
    auto ptx = points / _xps.size();
    for (auto& x : _xps)
    {
        this->set_xp(x.first, ptx);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::operator-= (AdvancementTable::xp points)
{
    auto ptx = -points / _xps.size();
    for (auto& x : _xps)
    {
        this->set_xp(x.first, ptx);
    }
    return (*this);
}

Adndtk::Experience& Adndtk::Experience::set_xp(const Adndtk::Defs::character_class& cls, const AdvancementTable::xp& xp)
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

    if (xp < 0 && _xps[cls] < std::llabs(xp))
    {
        _xps[cls] = 0;
        _levels[cls] = 0;
        return *this;
    }

    auto advTable = Cyclopedia::get_instance().advancement_table();
    AdvancementTable::level currentLevel = advTable.get_level(cls, _xps[cls]);

    AdvancementTable::xp newXP = _xps[cls] + xp;
    AdvancementTable::level newLevel = advTable.get_level(cls, newXP);

    _xps[cls] = newXP;
    if (currentLevel != newLevel)
    {
        _levels[cls] = newLevel;
    }
    return (*this);
}

const Adndtk::AdvancementTable::xp& Adndtk::Experience::xp(const Adndtk::Defs::character_class& cls) const
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _xps.at(cls);
}

const Adndtk::AdvancementTable::level& Adndtk::Experience::level(const Adndtk::Defs::character_class& cls) const
{
    if (_xps.find(cls) == _xps.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }
    return _levels.at(cls);
}

const Adndtk::AdvancementTable::xp& Adndtk::Experience::xp() const
{
    return xp(_cls);
}

const Adndtk::AdvancementTable::level& Adndtk::Experience::level() const
{
    return level(_cls);
}