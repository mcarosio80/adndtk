#ifndef SKILLS_H
#define SKILLS_H

#include <iostream>
#include <iomanip>
#include <optional>
#include <stdexcept>

#include <defs.h>

namespace Adndtk
{
    class SkillValue
    {
    public:
        SkillValue();
        SkillValue(const Defs::skill& type, const short& value, const std::optional<short>& excValue = std::nullopt);

        bool hasExceptionalStrength() const;
        std::optional<short> exceptionalStrength() const;
        inline const Defs::skill& type() const { return _skillType; };

        friend std::ostream& operator<< (std::ostream& out, const SkillValue& value)
        {
            out << value._skillValue;
            if (value.hasExceptionalStrength())
            {
                out << "/" << std::setfill('0') << std::setw(2) << value._exceptionalValue.value();
            }
            return out;
        }

        operator short() const
        {
            return _skillValue;
        }

        SkillValue& operator=(const SkillValue& val);
        SkillValue& operator=(const short& val);
        SkillValue& operator+=(const short& val);
        SkillValue& operator-=(const short& val);
        bool operator==(const SkillValue& val) const;
        bool operator<(const SkillValue& val) const;

        bool operator<=(const SkillValue& val) const { return (*this) < val || (*this) == val; }
        bool operator>(const SkillValue& val) const { return !((*this) <= val); }
        bool operator>=(const SkillValue& val) const { return !((*this) < val); }
        bool operator!=(const SkillValue& val) const { return !((*this) == val); }

    private:
        Defs::skill             _skillType;
        short                   _skillValue;
        std::optional<short>    _exceptionalValue;
    };
}

#endif // SKILLS_H