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
        bool setExceptionalStrength(const short& newValue);
        std::optional<short> exceptionalStrength() const;
        inline const Defs::skill& type() const { return _skillType; };

        friend std::ostream& operator<< (std::ostream& out, const SkillValue& value)
        {
            out << value._skillValue;
            if (value.hasExceptionalStrength())
            {
                out << "/";
                std::ios prevStatus(NULL);
                prevStatus.copyfmt(out);
                if (value._exceptionalValue.value() == 100)
                {
                    out << "00";
                }
                else
                {
                    out << std::right << std::setfill('0') << std::setw(2) << value._exceptionalValue.value();
                }

                out.copyfmt(prevStatus);
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
        std::partial_ordering operator<=>(const SkillValue& val) const
        {
            if (_skillType != val._skillType)
                return std::partial_ordering::unordered;

            if (_skillType == Defs::skill::strength && _skillValue == 18 && val._skillValue == 18)
            {
                if (_exceptionalValue < val._exceptionalValue)
                {
                    return std::partial_ordering::less;
                }
                else if (_exceptionalValue > val._exceptionalValue)
                {
                    return std::partial_ordering::greater;
                }
            }
            else
            {
                if (_skillValue < val._skillValue)
                {
                    return std::partial_ordering::less;
                }
                else if (_skillValue > val._skillValue)
                {
                    return std::partial_ordering::greater;
                }
            }
            return std::partial_ordering::equivalent;
        }

    private:
        Defs::skill             _skillType;
        short                   _skillValue;
        std::optional<short>    _exceptionalValue;
    };
}

#endif // SKILLS_H