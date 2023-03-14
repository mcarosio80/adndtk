#include <skills.h>

Adndtk::SkillValue::SkillValue()
    : _skillType{Defs::skill::strength}, _skillValue{9}, _exceptionalValue{std::nullopt}
{
}

Adndtk::SkillValue::SkillValue(const Defs::skill& type, const short& value, const std::optional<short>& excValue)
    : _skillType{type}, _skillValue{value}, _exceptionalValue{excValue}
{
}

bool Adndtk::SkillValue::hasExceptionalStrength() const
{
    return _skillType == Defs::skill::strength
            && _skillValue == 18
            && _exceptionalValue.has_value();
}

Adndtk::SkillValue& Adndtk::SkillValue::operator=(const short& val)
{
    if (val < 0 || val > 25)
        throw std::runtime_error("Invalid skill value");

    _skillValue = val;
    _exceptionalValue = std::nullopt;

    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator=(const Adndtk::SkillValue& val)
{
    _skillValue = val._skillValue;
    _skillType = val._skillType;
    _exceptionalValue = val._exceptionalValue;

    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator+=(const short& val)
{
    short newValue = this->_skillValue + val;
    if (newValue < 0 || newValue > 25)
        throw std::runtime_error("Unable to modifiy skill value");

    _skillValue = newValue;
    _exceptionalValue = std::nullopt;

    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator+=(const Adndtk::SkillValue& val)
{
    (*this) += val._skillValue;
    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator-=(const short& val)
{
    (*this) += -val;
    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator-=(const Adndtk::SkillValue& val)
{
    (*this) += -val._skillValue;
    return (*this);
}

bool Adndtk::SkillValue::operator==(const Adndtk::SkillValue& val) const
{
    if (_skillValue != val._skillValue) // Comparison is impossible
        return false;
    if (_skillType != Defs::skill::strength)
        return _skillValue == val._skillValue;
    if (_skillValue == 18)
        return _skillValue == val._skillValue && _exceptionalValue == val._exceptionalValue;
    else
        return _skillValue == val._skillValue;
}

bool Adndtk::SkillValue::operator<(const Adndtk::SkillValue& val) const
{
    if (_skillValue != val._skillValue)
        throw std::runtime_error("Unable to compare different skills");

    if (_skillType == Defs::skill::strength && _skillValue == 18)
        return _skillValue == val._skillValue && _exceptionalValue < val._exceptionalValue;
    else
        return _skillValue < val._skillValue;
}