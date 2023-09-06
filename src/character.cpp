#include <character.h>
#include <cyclopedia.h>
#include <skill_creator.h>
#include <options.h>


Adndtk::Character::Character(const std::string& name, const Defs::character_class& cls,
                const Defs::race& raceId, const Defs::sex& sexId,
                const std::optional<Defs::deity>& deityId/*=std::nullopt*/)
    : _name{name}, _cls{cls}, _race{raceId}, _sex{sexId}, _deity{deityId},
    _xp{cls, raceId}, _hp{cls}, _thaco{},
    _money{Cyclopedia::get_instance().get_class_type(cls)}, _inventory{},
    _forwardEvent{true}
{
    _hp += [&] (const HPChangeType &chgType, const HP &prevHP, const XP &newHP) -> void
    {
        this->on_change_hp(chgType, prevHP, newHP);
    };
    
    auto cbk = [&] (const SkillValue& oldVal, const SkillValue& newVal) -> void
    {
        _hp.update_constitution(oldVal, newVal);
    };
    _sklChgCbk.push_back(cbk);

    _xp += [&] (const Defs::character_class &cls, const XPChangeType &chgType,
                const XP &prevXP, const ExperienceLevel &prevLvl,
                const XP &newXP, const ExperienceLevel &newLvl) -> void
    {
        this->on_change_xp(cls, chgType, prevXP, prevLvl, newXP, newLvl);
    };
    auto sklGenMethod = static_cast<SkillGenerationMethod>(OptionalRules::get_instance().option<int>(Option::skills_generation_method));
    
    for (auto skl : {Defs::skill::strength, Defs::skill::dexterity, Defs::skill::constitution,
                    Defs::skill::intelligence, Defs::skill::wisdom, Defs::skill::charisma})
    {
        auto val = SkillCreator::create(skl, _cls, _race, sklGenMethod);
        _skills[skl] = val;
    }
}

std::vector<Adndtk::Defs::character_class> Adndtk::Character::get_class() const
{
    return Cyclopedia::get_instance().split(_cls);
}

Adndtk::Character& Adndtk::Character::operator+=(const OnSkillValueChange& cbk)
{
    _sklChgCbk.push_back(cbk);
    return (*this);
}

void Adndtk::Character::change_skill(const SkillValue& newValue)
{
    SkillValue oldValue{_skills[newValue.type()]};
    _skills[newValue.type()] = newValue;
    notify_all(oldValue, newValue);
}

void Adndtk::Character::gain_xp(const XP& xpValue, const std::optional<Defs::character_class>& cls/*=std::nullopt*/)
{
    if (cls.has_value())
    {
        Defs::character_class chrCls = cls.value();
        CharacterExperience pts(chrCls);
        pts.set(chrCls, 0, xpValue);
        _xp += pts;
    }
    else
    {
        _xp += xpValue;
    }
}

void Adndtk::Character::lose_xp(const XP& xpValue, const std::optional<Defs::character_class>& cls/*=std::nullopt*/)
{
    if (cls.has_value())
    {
        Defs::character_class chrCls = cls.value();
        CharacterExperience pts(chrCls);
        pts.set(chrCls, 0, -xpValue);
        _xp += pts;
    }
    else
    {
        _xp -= xpValue;
    }
}

void Adndtk::Character::gain_level(const ExperienceLevel& lvl, const std::optional<Defs::character_class>& cls/*=std::nullopt*/)
{
    if (cls.has_value())
    {
        Defs::character_class chrCls = cls.value();
        _xp.add(chrCls, lvl);
    }
    else
    {
        _xp.add(lvl);
    }
}

void Adndtk::Character::lose_level(const ExperienceLevel& lvl, const std::optional<Defs::character_class>& cls/*=std::nullopt*/)
{
    if (cls.has_value())
    {
        Defs::character_class chrCls = cls.value();
        _xp.subtract(chrCls, lvl);
    }
    else
    {
        _xp.subtract(lvl);
    }
}

Adndtk::CharacterExperience Adndtk::Character::experience()
{
    CharacterExperience xps{_cls};

    for (auto& xp : xps)
    {
        auto points = _xp.xp(xp.first);
        auto level = _xp.level(xp.first);
        xps.set(xp.first, level, points);
    }

    return xps;
}

void Adndtk::Character::on_change_xp(const Defs::character_class &cls, const XPChangeType &chgType,
                                          const XP &prevXP, const ExperienceLevel &prevLvl,
                                          const XP &newXP, const ExperienceLevel &newLvl)
{
    if (!_forwardEvent)
    {
        return;
    }
    _forwardEvent = false;
    switch (chgType)
    {
        case XPChangeType::level_down:
            _hp.shrink(cls, prevLvl - newLvl);
            break;
        case XPChangeType::level_up:
            _hp.increase(cls, newLvl - prevLvl);
            break;
        case XPChangeType::level_zero:
            break;
        case XPChangeType::death:
            break;
    };
    _forwardEvent = true;
}

void Adndtk::Character::on_change_hp(const HPChangeType &chgType, const HP &prevHP, const XP &newHP)
{
    if (!_forwardEvent)
    {
        return;
    }
    _forwardEvent = false;
    switch (chgType)
    {
        case HPChangeType::none:
            break;
        case HPChangeType::wounded:
            break;
        case HPChangeType::healed:
            break;
        case HPChangeType::dead:
            break;
    };
    _forwardEvent = true;
}

void Adndtk::Character::notify_all(const SkillValue& precValue, const SkillValue& currValue)
{
    for (auto& cbk : _sklChgCbk)
    {
        cbk(precValue, currValue);
    }
}

Adndtk::HPChangeType Adndtk::Character::wound(const HP& hps)
{
    HPChangeType result{HPChangeType::none};
    _hp -= hps;
    return result;
}

Adndtk::HPChangeType Adndtk::Character::cure(const HP& hps)
{
    HPChangeType result{HPChangeType::none};
    _hp += hps;
    return result;
}

Adndtk::HPChangeType Adndtk::Character::raise()
{
    HPChangeType result{HPChangeType::none};

    return result;
}

Adndtk::HPChangeType Adndtk::Character::restore()
{
    HPChangeType result{HPChangeType::none};

    return result;
}
