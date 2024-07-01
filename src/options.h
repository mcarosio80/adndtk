#ifndef OPTIONS_H
#define OPTIONS_H

#include <optional>
#include <map>
#include <variant>
#include <string>
#include <cstdint>

namespace Adndtk
{
    enum class Option
    {
        max_score_for_hd,
        scribe_scroll_always_succeeds,
        encumbrance,
        skills_generation_method,
        max_starting_money,
        enable_bonus_xp_for_high_prime_requisites,
        check_for_deity_selection,
        apply_aging_effect,
        apply_encumbrance,
        unlimited_store_availability,   // unlimited money
        unlimited_store_supply,         // supplied goods are available unlimitedly
        store_profit_margin_percentage,
        store_min_cp_starting_amount,
        store_max_cp_starting_amount,
        store_min_sp_starting_amount,
        store_max_sp_starting_amount,
        store_min_ep_starting_amount,
        store_max_ep_starting_amount,
        store_min_gp_starting_amount,
        store_max_gp_starting_amount,
        store_min_pp_starting_amount,
        store_max_pp_starting_amount,
        treasure_components_always_present
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

		static bool     _initialised;
        std::map<Option, std::variant<uint32_t, int, double, bool, std::string>>
                        _options;
    };
}

#endif // OPTIONS_H