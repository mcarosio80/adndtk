#include <skills.h>
#include <config.h>

Adndtk::SkillValue::SkillValue()
    : _skillType{Defs::skill::strength}, _skillValue{9}, _exceptionalValue{std::nullopt}
{
}

Adndtk::SkillValue::SkillValue(const Defs::skill& type, const short& value, const std::optional<short>& excValue)
    : _skillType{type}, _skillValue{value}, _exceptionalValue{excValue}
{
    if (excValue.has_value())
    {
        if (type != Defs::skill::strength || value != 18)
        {
            ErrorManager::get_instance().error("Exceptional strength not allowed");
        }
        if (type == Defs::skill::strength && (excValue.value() < 1 || excValue.value() > 100))
        {
            ErrorManager::get_instance().error("Invalid value for exceptional strength");
        }
    }
}

bool Adndtk::SkillValue::hasExceptionalStrength() const
{
    return _skillType == Defs::skill::strength
            && _skillValue == 18
            && _exceptionalValue.has_value();
}

std::optional<short> Adndtk::SkillValue::exceptionalStrength() const
{
    if (_skillType == Defs::skill::strength && _skillValue == 18)
    {
        return _exceptionalValue;
    }
    return std::nullopt;
}

Adndtk::SkillValue& Adndtk::SkillValue::operator=(const short& val)
{
    if (val < 0 || val > 25)
        ErrorManager::get_instance().error("Invalid skill value");

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
        ErrorManager::get_instance().error("Unable to modifiy skill value");

    _skillValue = newValue;
    //_exceptionalValue = std::nullopt;

    return (*this);
}

Adndtk::SkillValue& Adndtk::SkillValue::operator-=(const short& val)
{
    (*this) += -val;
    return (*this);
}

bool Adndtk::SkillValue::operator==(const Adndtk::SkillValue& val) const
{
    if (_skillType != val._skillType) // Comparison is impossible
        ErrorManager::get_instance().error("Unable to compare different skills");
    if (_skillType != Defs::skill::strength)
        return _skillValue == val._skillValue;
    if (_skillValue == 18)
        return _skillValue == val._skillValue && _exceptionalValue == val._exceptionalValue;
    else
        return _skillValue == val._skillValue;
}

bool Adndtk::SkillValue::operator<(const Adndtk::SkillValue& val) const
{
    if (_skillType != val._skillType)
        ErrorManager::get_instance().error("Unable to compare different skills");

    if (_skillType == Defs::skill::strength && _skillValue == 18 && val == 18)
        return _skillValue == val._skillValue && _exceptionalValue < val._exceptionalValue;
    else
        return _skillValue < val._skillValue;
}