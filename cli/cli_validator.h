#ifndef ADND_CLI_OPTION_VALIDATOR_H
#define ADND_CLI_OPTION_VALIDATOR_H

#include <optional>
#include <string>

#include <common_types.h>

namespace CliTools
{
    class AdndtkCliValidator
    {
    public:
        AdndtkCliValidator();

        // std::optional<EobLib::Version> validate_version(const std::string& stringValue) const;
        // std::optional<EobLib::Race> validate_race(const std::string& stringValue) const;
        // std::optional<EobLib::Sex> validate_sex(const char& stringValue) const;
        // std::optional<EobLib::Alignment> validate_alignment(const std::string& stringValue) const;
        // std::optional<EobLib::Class> validate_class_id(const std::string& stringValue) const;
        // std::optional<std::pair<short, std::optional<short>>> validate_strength(const std::string& stringValue) const;
        auto validate_skill(const std::string& stringValue) const -> std::optional<short>;
        auto validate_generation_method(const std::string& value) const -> std::optional<Adndtk::SkillGenerationMethod>;

        // template<typename _Type>
        // std::optional<std::tuple<_Type, std::optional<_Type>, std::optional<_Type>>> validate_three_values(const std::string& val) const
        // {
        //     std::regex patternSingle("(\\d+)", std::regex_constants::ECMAScript);
        //     std::regex patternDouble("(\\d+)/(\\d+)", std::regex_constants::ECMAScript);
        //     std::regex patternTriple("(\\d+)/(\\d+)/(\\d+)", std::regex_constants::ECMAScript);

        //     std::smatch matches;
        //     _Type first{0};
        //     std::optional<_Type> second{std::nullopt};
        //     std::optional<_Type> third{std::nullopt};

        //     if (std::regex_match(val, matches, patternTriple))
        //     {
        //         first = std::stoi(matches[1]);
        //         second = std::stoi(matches[2]);
        //         third = std::stoi(matches[2]);
        //     }
        //     else if (std::regex_match(val, matches, patternDouble))
        //     {
        //         first = std::stoi(matches[1]);
        //         second = std::stoi(matches[2]);
        //     }
        //     else if (std::regex_match(val, matches, patternSingle))
        //     {
        //         first = std::stoi(matches[1]);
        //     }
        //     else
        //     {
        //         return std::nullopt;
        //     }

        //     return std::make_tuple(first, second, third);
        // };
    };
} // namespace CliTools

#endif //ADND_CLI_OPTION_VALIDATOR_H