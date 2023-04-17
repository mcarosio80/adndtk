#include <hit_points.h>
#include <cyclopedia.h>
#include <options.h>
#include <common_types.h>
#include <dice.h>

Adndtk::HitPoints::HitPoints()
    : _cls{}, _currentHP{}
{
}

Adndtk::HitPoints::HitPoints(const Defs::character_class& cls)
    : _cls{cls}, _currentHP{}
{
    auto classes = Cyclopedia::get_instance().split(cls);

    for (auto& c : classes)
    {
        int clsId = static_cast<int>(c);
        auto clsInfo= Cyclopedia::get_instance().exec_prepared_statement<int>(Adndtk::Query::select_character_class, clsId);
        int clsType = std::stoi(clsInfo[0]["class_type_id"].value());

        auto clsTypeInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Adndtk::Query::select_character_class_type, clsType);
        int hdFaces = std::stoi(clsTypeInfo[0]["hit_dice"].value());

        _hitDice[c] = static_cast<Defs::die>(hdFaces);
        
        HP pts = generate_hp(c);
        _hps[c].push_back(pts);
        _levels[c] = 1;
    }
    
    /*The character's hit points are the average of all his Hit Dice rolls. When the character is
    first created, the player rolls hit points for each class separately, totals them up, then
    divides by the number of dice rolled (round fractions down).
    */
    for (auto& h : _hps)
    {
        _currentHP += h.second[0];
    }
    _currentHP = std::div(_currentHP, _hps.size()).quot;
}

Adndtk::HitPoints::operator HP() const
{
    return _currentHP;
}

Adndtk::HitPoints& Adndtk::HitPoints::operator+=(const Adndtk::HP& hp)
{
    HP oldHP = _currentHP;
    _currentHP += std::min(hp, total() - _currentHP);
    notify_all_listeners(oldHP, _currentHP);
    return (*this);
}

Adndtk::HitPoints& Adndtk::HitPoints::operator-=(const Adndtk::HP& hp)
{
    HP oldHP = _currentHP;
    _currentHP -= std::min(hp, _currentHP);
    notify_all_listeners(oldHP, _currentHP);
    return (*this);
}

Adndtk::HitPoints& Adndtk::HitPoints::operator+=(const Adndtk::OnHPChange& cbk)
{
    _cbks.push_back(cbk);
    return (*this);
}

Adndtk::HP Adndtk::HitPoints::total() const
{
    HP total{0};

    for (auto& x : _hps)
    {
        total += ((x.second.size() > 0)) ? x.second[0] : 1;
    }
    total = std::div(total, _hps.size()).quot;

    for (auto& x : _hps)
    {
        for (int i=1; i<_levels.at(x.first); ++i)
        {
            HP pts = std::div(x.second[i], _hps.size()).quot;
            total += std::max(1, static_cast<int>(pts));
        }
    }
    return total;
}

const Adndtk::HP& Adndtk::HitPoints::current() const
{
    return _currentHP;
}

// Adndtk::HitPoints& Adndtk::HitPoints::shrink(const ExperienceLevel& count/*=1*/)
// {
//     for (ExperienceLevel lvl=0; lvl<count; ++lvl)
//     {
//         Defs::character_class cls = max_level();
//         shrink(cls, 1);
//     }
//     return (*this);
// }

Adndtk::HitPoints& Adndtk::HitPoints::shrink(const Defs::character_class& cls, const ExperienceLevel& count/*=1*/)
{
    if (_levels.find(cls) == _levels.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
    }

    HP prevTotal = total();
    if (count < _levels[cls])
    {
        _levels[cls] -= count;
    }
    else if (count == _levels[cls])
    {
        _levels[cls] = 0;
        _hps[cls].clear();
    }
    else
    {
        _levels.erase(cls);
        _hps.erase(cls);
        _hitDice.erase(cls);
    }

    HP prevHP = _currentHP;
    _currentHP -= prevTotal - total();
    notify_all_listeners(prevHP, _currentHP);
    return (*this);
}

// Adndtk::Defs::character_class Adndtk::HitPoints::min_level()
// {
//     auto selected = *_levels.begin();
//     Defs::character_class cls{selected.first};
//     for (auto& x : _levels)
//     {
//         if (x.second < selected.second)
//         {
//             cls = x.first;
//         }
//     }
//     return cls;
// }

// Adndtk::Defs::character_class Adndtk::HitPoints::max_level()
// {
//     auto selected = *_levels.begin();
//     Defs::character_class cls{selected.first};
//     for (auto& x : _levels)
//     {
//         if (x.second > selected.second)
//         {
//             cls = x.first;
//         }
//     }
//     return cls;
// }

// Adndtk::HitPoints& Adndtk::HitPoints::increase(const ExperienceLevel& count/*=1*/)
// {
//     for (ExperienceLevel lvl=0; lvl<count; ++lvl)
//     {
//         Defs::character_class cls = min_level();
//         increase(cls, 1);
//     }
//     return (*this);
// }

Adndtk::HitPoints& Adndtk::HitPoints::increase(const Defs::character_class& cls, const ExperienceLevel& count/*=1*/)
{
    if (_hitDice.find(cls) == _hitDice.end())
    {
        ErrorManager::get_instance().error("Invalid class specified");
        return (*this);
    }
    if (_levels[cls] == 0)
    {
        ErrorManager::get_instance().error("Unable to advance from level 0");
        return (*this);
    }

    HP prevTotal = total();
    auto newLevel = _levels[cls] + count;
    if (newLevel <= _hps[cls].size())
    {
        _levels[cls] = newLevel;
    }
    else
    {
        auto numLevels = newLevel - _hps[cls].size();
        for (ExperienceLevel s=0; s<numLevels; ++s)
        {
            HP pts = generate_hp(cls);
            _hps[cls].push_back(pts);
        }
        _levels[cls] += numLevels;
    }

    HP prevHP = _currentHP;
    _currentHP += total() - prevTotal;
    notify_all_listeners(prevHP, _currentHP);
    return (*this);
}

Adndtk::ExperienceLevel Adndtk::HitPoints::level()
{
    auto selected = *_levels.begin();
    ExperienceLevel exp{selected.second};
    for (auto& x : _levels)
    {
        if (x.second > selected.second)
        {
            exp = x.second;
        }
    }
    return exp;
}

Adndtk::ExperienceLevel Adndtk::HitPoints::level(const Defs::character_class& cls)
{
    return _levels[cls];
}

Adndtk::HP Adndtk::HitPoints::generate_hp(const Adndtk::Defs::character_class& cls)
{
    if (OptionalRules::get_instance().option<bool>(Option::max_score_for_hd))
    {
        return static_cast<HP>(_hitDice[cls]);
    }
    Die d{_hitDice[cls]};
    return d;
}

void Adndtk::HitPoints::notify_all_listeners(const HP& prevHP, const HP& newHP)
{
    HPChangeType chgType = HPChangeType::none;
    if (newHP == 0)
    {
        chgType = HPChangeType::dead;
    }
    else if (prevHP > newHP)
    {
        chgType = HPChangeType::wounded; 
    }
    else if (prevHP < newHP)
    {
        chgType = HPChangeType::healed; 
    }

    for (auto& cbk : _cbks)
    {
        cbk(chgType, prevHP, newHP);
    }
}