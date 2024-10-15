#include <cli_validator.h>

#include <regex>

namespace CliTools
{

AdndtkCliValidator::AdndtkCliValidator()
{
}

// std::optional<std::pair<short, std::optional<short>>> EobDataValidator::validate_strength(const std::string& stringValue) const
// {
//     std::regex pattern("(2[0-5]|1[0-9]|[1-9])", std::regex_constants::ECMAScript);
//     std::regex patternExc("(18)/([0-9][0-9])", std::regex_constants::ECMAScript);
    
//     std::smatch matches;
//     short strengthValue{0};
//     short excStrengthValue{0};

//     if (std::regex_match(stringValue, matches, pattern))
//     {
//         strengthValue = std::stoi(matches[1]);
//     }
//     else if (std::regex_match(stringValue, matches, patternExc))
//     {
//         strengthValue = std::stoi(matches[1]);
//         if (strengthValue == 18)
//         {
//             excStrengthValue = std::stoi(matches[2]);
//             excStrengthValue = (excStrengthValue == 0) ? 100 : excStrengthValue;
//         }
//     }
//     else
//     {
//         return std::nullopt;
//     }

//     return std::make_pair(strengthValue, excStrengthValue);
// }

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

} // namespace CliTools