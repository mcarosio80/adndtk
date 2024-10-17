#ifndef ADND_CLI_OPTION_VALIDATOR_H
#define ADND_CLI_OPTION_VALIDATOR_H

#include <optional>
#include <string>
#include <map>

#include <common_types.h>

namespace CliTools
{
    class AdndtkCliValidator
    {
    public:
        AdndtkCliValidator();

        auto validate_strength(const std::string& stringValue) const -> std::optional<std::pair<short, std::optional<short>>>;
        auto validate_skill(const std::string& stringValue) const -> std::optional<short>;
        auto validate_generation_method(const std::string& value) const -> std::optional<Adndtk::SkillGenerationMethod>;
        auto validate_skills_list(const std::string& value) const -> std::optional<std::map<Adndtk::Defs::skill, Adndtk::SkillValue>>;
    };
} // namespace CliTools

#endif //ADND_CLI_OPTION_VALIDATOR_H