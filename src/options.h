#ifndef OPTIONS_H
#define OPTIONS_H

#include <optional>
#include <map>
#include <variant>
#include <string>

namespace Adndtk
{
    enum class Option
    {
        max_score_for_hp
    };

    class OptionalRules
    {
    public:
        static OptionalRules& get_instance();
        OptionalRules(OptionalRules const&) = delete;
        void operator=(OptionalRules const&) = delete;

        std::optional<std::variant<int, double, bool, std::string>>& option(const Option& opt)
        {
            if (_options.find(opt) == _options.end())
            {
                _options[opt] = std::nullopt;
            }
            return _options[opt];
        }

    private:
        OptionalRules();
        ~OptionalRules();

		static bool                                                                     _initialised;
        std::map<Option, std::optional<std::variant<int, double, bool, std::string>>>   _options;
    };
}

#endif // OPTIONS_H