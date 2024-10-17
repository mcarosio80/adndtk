#include <cli_validator.h>

#include <regex>

namespace CliTools
{

AdndtkCliValidator::AdndtkCliValidator()
{
}

auto AdndtkCliValidator::validate_strength(const std::string& stringValue) const ->std::optional<std::pair<short, std::optional<short>>>
{
    std::regex pattern("(2[0-5]|1[0-9]|[1-9])", std::regex_constants::ECMAScript);
    std::regex patternExc("(18)/([0-9][0-9])", std::regex_constants::ECMAScript);
    
    std::smatch matches;
    short strengthValue{0};
    short excStrengthValue{0};

    if (std::regex_match(stringValue, matches, pattern))
    {
        strengthValue = std::stoi(matches[1]);
    }
    else if (std::regex_match(stringValue, matches, patternExc))
    {
        strengthValue = std::stoi(matches[1]);
        if (strengthValue == 18)
        {
            excStrengthValue = std::stoi(matches[2]);
            excStrengthValue = (excStrengthValue == 0) ? 100 : excStrengthValue;
        }
    }
    else
    {
        return std::nullopt;
    }

    return std::make_pair(strengthValue, excStrengthValue);
}

auto AdndtkCliValidator::validate_skill(const std::string& stringValue) const -> std::optional<short>
{
    std::regex pattern("(2[0-5]|1[0-9]|[1-9])", std::regex_constants::ECMAScript);
    
    std::smatch matches;
    short skillValue{0};

    if (std::regex_match(stringValue, matches, pattern))
    {
        skillValue = std::stoi(matches[1]);
    }
    else
    {
        return std::nullopt;
    }

    return skillValue;
}

auto AdndtkCliValidator::validate_generation_method(const std::string& stringValue) const -> std::optional<Adndtk::SkillGenerationMethod>
{
    if (stringValue == "standard")
    {
        return Adndtk::SkillGenerationMethod::standard;
    }
    else if (stringValue == "best4")
    {
        return Adndtk::SkillGenerationMethod::best_of_four;
    }
    else if (stringValue == "best-each")
    {
        return Adndtk::SkillGenerationMethod::best_of_each;
    }
    return std::nullopt;
}

auto AdndtkCliValidator::validate_skills_list(const std::string& value) const -> std::optional<std::map<Adndtk::Defs::skill, Adndtk::SkillValue>>
{
    std::regex pattern("(\\s*((\\d+/\\d+|(\\d+)|))\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+)\\s*,\\s*(\\d+))", std::regex_constants::ECMAScript);

    std::smatch matches;
    if (std::regex_match(value, matches, pattern))
    {
        auto strValue = validate_strength(matches[1]);
        auto dexValue = validate_skill(matches[2]);
        auto conValue = validate_skill(matches[3]);
        auto intValue = validate_skill(matches[4]);
        auto wisValue = validate_skill(matches[5]);
        auto chaValue = validate_skill(matches[6]);

        if (strValue.has_value() && dexValue.has_value() && conValue.has_value()
            && intValue.has_value() && wisValue.has_value() && chaValue.has_value())
        {
            std::map<Adndtk::Defs::skill, Adndtk::SkillValue> skillVals
            {
                {Adndtk::Defs::skill::strength, Adndtk::SkillValue{Adndtk::Defs::skill::strength, strValue.value().first, strValue.value().second}},
                {Adndtk::Defs::skill::dexterity, Adndtk::SkillValue{Adndtk::Defs::skill::dexterity, dexValue.value()}},
                {Adndtk::Defs::skill::constitution, Adndtk::SkillValue{Adndtk::Defs::skill::constitution, conValue.value()}},
                {Adndtk::Defs::skill::intelligence, Adndtk::SkillValue{Adndtk::Defs::skill::intelligence, intValue.value()}},
                {Adndtk::Defs::skill::wisdom, Adndtk::SkillValue{Adndtk::Defs::skill::wisdom, wisValue.value()}},
                {Adndtk::Defs::skill::charisma, Adndtk::SkillValue{Adndtk::Defs::skill::charisma, chaValue.value()}},
            };
            return skillVals;
        }
        else
        {
            return std::nullopt;
        }
    }
    else
    {
        return std::nullopt;
    }
}


} // namespace CliTools