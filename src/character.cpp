#include <character.h>
#include <cyclopedia.h>
#include <skill_creator.h>
#include <options.h>
#include <encumbrance.h>

#include <algorithm>


Adndtk::Character::Character(const std::string& name, const Defs::character_class& cls,
                const Defs::race& raceId, const Defs::moral_alignment& align, const Defs::sex& sexId,
                const std::optional<Defs::deity>& deityId/*=std::nullopt*/)
    : _name{name}, _cls{cls}, _race{raceId}, _align{align}, _sex{sexId}, _deity{deityId},
    _xp{cls, raceId}, _hp{cls}, _thaco{Cyclopedia::get_instance().get_class_type(cls)},
    _money{Cyclopedia::get_instance().get_class_type(cls)}, _inventory{},
    _forwardEvent{true}, _spellBook{cls, raceId}, _holySymbol{cls, deityId},
    _racialStats{raceId, sexId}
{
    if (!verify_moral_alignment())
    {
        throw std::runtime_error("Moral alignment is not compatible with the character class chosen");
    }
    if (!verify_worshipped_deity())
    {
        throw std::runtime_error("Worshipped deity is not compatible with the character class chosen");
    }

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

    for (auto& c : Cyclopedia::get_instance().split<Defs::character_class>(cls))
    {
        auto clsId = static_cast<int>(c);
        auto resSet = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_primary_skills, clsId);
        for (auto& r : resSet)
        {
            auto sklId = static_cast<Defs::skill>(r.as<int>("skill_id"));
            _primeRequisites[c].emplace(sklId);
        }
    }

    auto onAgingCbk = [&] (const Defs::skill& skl, const short& sklModifier) -> void
    {
        auto& skillVal = skill(skl);
        skillVal += sklModifier;
        change_skill(skillVal);
    };
    _racialStats += onAgingCbk;
}

std::vector<Adndtk::Defs::character_class> Adndtk::Character::get_classes() const
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
 
    if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls)
        && newValue.type() == Defs::skill::intelligence)
    {
        _spellBook.set_caster_intelligence(newValue);
    }
    if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls)
        && newValue.type() == Defs::skill::wisdom)
    {
        _holySymbol.set_caster_wisdom(newValue);
    }

    if (!OptionalRules::get_instance().option<bool>(Option::enable_bonus_xp_for_high_prime_requisites))
    {
        return;
    }

    for (auto& clsReqs : _primeRequisites)
    {
        double xpBonus = Const::xp_bonus_10;
        for (auto& skl : clsReqs.second)
        {
            if (_skills[skl] < Const::high_value_for_prime_requisite)
            {
                xpBonus = Const::xp_bonus_none;
            }
        }
        _xp.set_xp_bonus(clsReqs.first, xpBonus);
    }
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

Adndtk::CharacterExperience Adndtk::Character::experience() const
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

            if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(cls))
            {
                _spellBook.set_caster_level(newLvl);
            }
            if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(cls))
            {
                _holySymbol.set_caster_level(newLvl);
            }

            break;
        case XPChangeType::level_up:
            _hp.increase(cls, newLvl - prevLvl);
            
            if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(cls))
            {
                _spellBook.set_caster_level(newLvl);
            }
            if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(cls))
            {
                _holySymbol.set_caster_level(newLvl);
            }

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

const Adndtk::SpellBook& Adndtk::Character::spell_book() const
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("Spell book not available");
    }
    return _spellBook;
}

const Adndtk::HolySymbol& Adndtk::Character::holy_symbol() const
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls))
    {
        throw std::runtime_error("Spell book not available");
    }
    return _holySymbol;
}

bool Adndtk::Character::is_spell_caster() const
{
    return Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls)
        || Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls);
}

bool Adndtk::Character::cast_spell(const Defs::wizard_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("Unable to cast wizard spells");
    }
    return false;
}

bool Adndtk::Character::learn_spell(const Defs::wizard_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("Unable to learn wizard spells");
    }
    return false;
}

bool Adndtk::Character::memorise_spell(const Defs::wizard_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("The character is not a magic-user");
    }
    return false;
}

bool Adndtk::Character::remove_spell(const Defs::wizard_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("The character is not a magic-user");
    }
    return false;
}

bool Adndtk::Character::erase_spell(const Defs::wizard_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::wizard>(_cls))
    {
        throw std::runtime_error("The character is not a magic-user");
    }
    return false;
}

bool Adndtk::Character::cast_spell(const Defs::priest_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls))
    {
        throw std::runtime_error("Unable to cast priest spells");
    }
    return false;
}

bool Adndtk::Character::memorise_spell(const Defs::priest_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls))
    {
        throw std::runtime_error("The character is not a magic-user");
    }
    return false;
}

bool Adndtk::Character::remove_spell(const Defs::priest_spell& spellId)
{
    if (!Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls))
    {
        throw std::runtime_error("The character is not a magic-user");
    }
    return false;
}

bool Adndtk::Character::save_roll(const Defs::saving_throw& savId, const short& bonusMalus/*=0*/) const
{
    return SavingThrows::get_instance().roll(_cls, experience(), savId, bonusMalus);
}

const Adndtk::SavingScore& Adndtk::Character::save_score(const Defs::saving_throw& savId) const
{
    return SavingThrows::get_instance().get_score(_cls, experience(), savId);
}

Adndtk::Defs::attack_result Adndtk::Character::try_hit(const AC& ac, const short& bonusMalus/*=0*/) const
{
    return _thaco.try_hit(experience(), ac, bonusMalus);
}

bool Adndtk::Character::verify_moral_alignment() const
{
    auto aligns = Cyclopedia::get_instance().available_moral_alignments(_cls, _deity);
    return aligns.find(_align) != aligns.end();
}

bool Adndtk::Character::verify_worshipped_deity() const
{
    if (!OptionalRules::get_instance().option<bool>(Option::check_for_deity_selection))
    {
        return true;
    }
    
    if (Cyclopedia::get_instance().can_cast_as<Defs::character_class_type::priest>(_cls))
    {
        if (!_deity.has_value())
        {
            throw std::runtime_error("This character must worship a deity");
        }
        auto deities = Cyclopedia::get_instance().available_deities(_align);
        return deities.find(_deity.value()) != deities.end();
    }
    return true;
}

short Adndtk::Character::movement_factor() const
{
    if (!OptionalRules::get_instance().option<bool>(Option::apply_encumbrance))
    {
        return Encumbrance::get_instance().get_base_movement(_race);
    }
    auto strVal = _skills.at(Defs::skill::strength);
    auto mov = Encumbrance::get_instance().movement_factor(strVal, _race, _inventory.total_weight());
    return mov;
}