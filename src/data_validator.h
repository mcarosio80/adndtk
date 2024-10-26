#ifndef DATA_VALIDATOR_H
#define DATA_VALIDATOR_H

#include <optional>
#include <string>
#include <map>
#include <regex>

#include <common_types.h>

namespace Adndtk
{
    class DataValidator
    {
    public:
        DataValidator();

        auto validate_strength(const std::string& stringValue) const -> std::optional<std::pair<short, std::optional<short>>>;
        auto validate_skill(const std::string& stringValue) const -> std::optional<short>;
        auto validate_extended_skill(const std::string& stringValue) const -> std::optional<short>;
        auto validate_generation_method(const std::string& value) const -> std::optional<SkillGenerationMethod>;
        auto validate_skills_list(const std::string& value) const -> std::optional<std::map<Defs::skill, SkillValue>>;
        auto validate_class(const std::string& value) const -> std::optional<Defs::character_class>;

    private:
        auto validate_skill_value(const std::string& stringValue, const std::regex& pattern) const -> std::optional<short>;

    };
} // namespace Adndtk

#endif //DATA_VALIDATOR_H