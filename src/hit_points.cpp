#include <hit_points.h>
#include <cyclopedia.h>
#include <options.h>

Adndtk::HitPoints::HitPoints()
    : _cls{}
{
}

Adndtk::HitPoints::HitPoints(const Defs::character_class& cls)
    : _cls{cls}
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
        increase(c);
    }
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
    }

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
        _levels[cls] += numLevels;;
    }
    return (*this);
}

Adndtk::ExperienceLevel Adndtk::HitPoints::length()
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

Adndtk::ExperienceLevel Adndtk::HitPoints::length(const Defs::character_class& cls)
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