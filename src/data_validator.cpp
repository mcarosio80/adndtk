#include <data_validator.h>
#include <tables.h>

#include <sstream>

Adndtk::DataValidator::DataValidator()
{
}

auto Adndtk::DataValidator::validate_strength(const std::string& stringValue) const ->std::optional<std::pair<short, std::optional<short>>>
{
    std::regex pattern("\\s*(18)/([0-9][0-9])\\s*", std::regex_constants::ECMAScript);
    
    std::smatch matches;
    short strengthValue{0};
    short excStrengthValue{0};

    auto optSkill = validate_skill(stringValue);
    if (optSkill.has_value())
    {
        strengthValue = optSkill.value();
    }
    else if (std::regex_match(stringValue, matches, pattern))
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

auto Adndtk::DataValidator::validate_skill(const std::string& stringValue) const -> std::optional<short>
{
    std::regex pattern("\\s*(|1[0-8]|[3-9])\\s*", std::regex_constants::ECMAScript);
    return validate_skill_value(stringValue, pattern);
}

auto Adndtk::DataValidator::validate_extended_skill(const std::string& stringValue) const -> std::optional<short>
{
    std::regex pattern("\\s*(2[0-5]|1[0-9]|[1-9])\\s*", std::regex_constants::ECMAScript);
    return validate_skill_value(stringValue, pattern);
}

auto Adndtk::DataValidator::validate_skill_value(const std::string& stringValue, const std::regex& pattern) const -> std::optional<short>
{
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

auto Adndtk::DataValidator::validate_generation_method(const std::string& stringValue) const -> std::optional<SkillGenerationMethod>
{
    if (stringValue == "standard")
    {
        return SkillGenerationMethod::standard;
    }
    else if (stringValue == "best4")
    {
        return SkillGenerationMethod::best_of_four;
    }
    else if (stringValue == "best-each")
    {
        return SkillGenerationMethod::best_of_each;
    }
    return std::nullopt;
}

auto Adndtk::DataValidator::validate_skills_list(const std::string& value) const -> std::optional<std::map<Defs::skill, SkillValue>>
{
    std::stringstream ss{value};
    std::vector<std::string> tokens;
    std::string token;
    
    while (getline(ss, token, ','))
    {
        tokens.push_back(token);
    }

    auto strValue = validate_strength(tokens[0]);
    auto dexValue = validate_skill(tokens[1]);
    auto conValue = validate_skill(tokens[2]);
    auto intValue = validate_skill(tokens[3]);
    auto wisValue = validate_skill(tokens[4]);
    auto chaValue = validate_skill(tokens[5]);

    if (!strValue.has_value() || !dexValue.has_value() || !conValue.has_value()
        || !intValue.has_value() || !wisValue.has_value() || !chaValue.has_value())
    {
        return std::nullopt;
    }

    auto excValue = strValue.value().second;
    if (excValue.has_value() && excValue.value() != 0 && strValue.value().first != 18)
    {
        return std::nullopt;
    }

    std::map<Defs::skill, SkillValue> skillVals{};
    if (excValue.has_value() && excValue.value() != 0 && strValue.value().first == 18)
    {
        skillVals[Defs::skill::strength] = SkillValue{Defs::skill::strength, strValue.value().first, strValue.value().second};
    }
    else
    {
        skillVals[Defs::skill::strength] = SkillValue{Defs::skill::strength, strValue.value().first};
    }
    skillVals[Defs::skill::dexterity] = SkillValue{Defs::skill::dexterity, dexValue.value()};
    skillVals[Defs::skill::constitution] = SkillValue{Defs::skill::constitution, conValue.value()};
    skillVals[Defs::skill::intelligence] = SkillValue{Defs::skill::intelligence, intValue.value()};
    skillVals[Defs::skill::wisdom] = SkillValue{Defs::skill::wisdom, wisValue.value()};
    skillVals[Defs::skill::charisma] = SkillValue{Defs::skill::charisma, chaValue.value()};

    return skillVals;
}

auto Adndtk::DataValidator::validate_class(const std::string& value) const -> std::optional<Defs::character_class>
{
    auto classes = Adndtk::Tables::character_class::fetch_all();
    for (auto& c : classes)
    {
        if (value == c.long_name || value == c.short_name || value == c.acronym)
        {
            return std::make_optional(c.id);
        }
    }

    return std::nullopt;
}