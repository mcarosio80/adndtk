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
        max_score_for_hd,
        scribe_scroll_always_succeeds,
        encumbrance,
        skills_generation_method,
        max_starting_money
    };

    class OptionalRules
    {
    public:
        static OptionalRules& get_instance();
        OptionalRules(OptionalRules const&) = delete;
        void operator=(OptionalRules const&) = delete;

        template<typename _T>
        _T& option(const Option& opt)
        {
            return std::get<_T>(_options[opt]);
        }

    private:
        OptionalRules();
        ~OptionalRules();

        void set_default_options_values();

		static bool                                                     _initialised;
        std::map<Option, std::variant<int, double, bool, std::string>>  _options;
    };
}

#endif // OPTIONS_H