#include <hit_points.h>
#include <cyclopedia.h>
#include <options.h>
#include <dice.h>
#include <skill_stats.h>
#include <adnd_errors.h>

Adndtk::HitPoints::HitPoints()
    : _cls{}, _currentHP{}, _constitutionAdjustment{std::nullopt}
{
}

Adndtk::HitPoints::HitPoints(const Defs::character_class& cls)
    : _cls{cls}, _currentHP{}, _constitutionAdjustment{std::nullopt}
{
    auto classes = Cyclopedia::get_instance().split(cls);

    for (auto& clsId : classes)
    {
        auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(Adndtk::Query::select_character_class, clsId);
        auto& clsInfo = rs[0];
        auto clsType = clsInfo.as<Defs::character_class_type>("class_type_id");

        auto clsTypeInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class_type>(Adndtk::Query::select_character_class_type, clsType);
        _hitDice[clsId] = clsTypeInfo[0].as<Defs::die>("hit_dice");
        _titleLevel[clsId] = clsTypeInfo[0].as<ExperienceLevel>("title_level");
        _hpAfterTitle[clsId] = clsTypeInfo[0].as<HP>("hp_after_title");
        
        HP pts = generate_hp(clsId, 1);
        _hps[clsId].push_back(pts);
        _levels[clsId] = 1;
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

void Adndtk::HitPoints::update_constitution(const SkillValue& oldVal, const SkillValue& newVal)
{
    if (oldVal.type() != Defs::skill::constitution || newVal.type() != Defs::skill::constitution)
    {
        return;
    }
    
    auto oldStats = SkillStats::get_instance().get_constitution_stats(oldVal);
    auto newStats = SkillStats::get_instance().get_constitution_stats(newVal);
    int oldHpAdj{0};
    int newHpAdj{0};

    if (Cyclopedia::get_instance().is_type_of<Defs::character_class_type::warrior>(_cls))
    {
        oldHpAdj = oldStats.hit_point_adjustment_warriors.value_or(oldStats.hit_point_adjustment);
        newHpAdj = newStats.hit_point_adjustment_warriors.value_or(newStats.hit_point_adjustment);
    }
    else
    {
        oldHpAdj = oldStats.hit_point_adjustment;
        newHpAdj = newStats.hit_point_adjustment;
    }

    if (_constitutionAdjustment.has_value())
    {
        _constitutionAdjustment = _constitutionAdjustment.value() - oldHpAdj + newHpAdj;
    }
    else
    {
        _constitutionAdjustment = newHpAdj;
    }
}

Adndtk::HitPoints::operator HP() const
{
    HP conBonus{_constitutionAdjustment.value_or(0)};
    HP lvlBonus = constitution_bonus();
    for (auto& c : _hps)
    {
        if (_levels.at(c.first) <= _titleLevel.at(c.first))
        {
            conBonus += lvlBonus * _hps.size()-1;
        }
    }
    return _currentHP + conBonus;
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
            total += std::max<HP>(1, pts);
        }
    }
    return total;
}

Adndtk::HP Adndtk::HitPoints::current() const
{
    HP conBonus{_constitutionAdjustment.value_or(0)};
    HP lvlBonus = constitution_bonus();
    for (auto& c : _hps)
    {
        auto conBonusCount = std::min<ExperienceLevel>(_levels.at(c.first), _titleLevel.at(c.first)) - 1;
        conBonus += lvlBonus * conBonusCount;
    }
    return _currentHP + conBonus;
}

Adndtk::HitPoints& Adndtk::HitPoints::shrink(const Defs::character_class& cls, const ExperienceLevel& count/*=1*/)
{
    if (_levels.find(cls) == _levels.end())
    {
        throw InvalidClassException(cls);
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

Adndtk::HitPoints& Adndtk::HitPoints::increase(const Defs::character_class& cls, const ExperienceLevel& count/*=1*/)
{
    if (_hitDice.find(cls) == _hitDice.end())
    {
        throw InvalidClassException(cls);
    }
    if (_levels[cls] == 0)
    {
        throw CharacterException("Unable to advance from level 0", cls);
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
            //HP bonus = (newLevel <= _titleLevel[cls]) ? constitution_bonus() : 0;
            HP pts = generate_hp(cls, newLevel);// + bonus;
            _hps[cls].push_back(pts);
        }
        _levels[cls] = newLevel;
    }

    HP prevHP = _currentHP;
    _currentHP += total() - prevTotal;
    notify_all_listeners(prevHP, _currentHP);
    return (*this);
}

Adndtk::ExperienceLevel Adndtk::HitPoints::level() const
{
    auto selected = *_levels.cbegin();
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

Adndtk::ExperienceLevel Adndtk::HitPoints::level(const Defs::character_class& cls) const
{
    return _levels.at(cls);
}

Adndtk::HP Adndtk::HitPoints::generate_hp(const Adndtk::Defs::character_class& cls, const ExperienceLevel& lvl)
{
    if (lvl > _titleLevel[cls])
    {
        return _hpAfterTitle[cls];
    }
    if (OptionalRules::get_instance().get_option<bool>(Option::max_score_for_hd))
    {
        return static_cast<HP>(_hitDice[cls]);
    }
    return DiceSet::get_instance().roll(1, _hitDice[cls]);
}

Adndtk::HP Adndtk::HitPoints::constitution_bonus() const
{
    if (_hps.size() == 0)
    {
        return 0;
    }

    HP conMod = std::div(_constitutionAdjustment.value_or(0), _hps.size()).quot;
    return conMod;
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