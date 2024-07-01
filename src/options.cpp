#include <options.h>
#include <skill_creator.h>

bool Adndtk::OptionalRules::_initialised = false;


Adndtk::OptionalRules& Adndtk::OptionalRules::get_instance()
{
    static OptionalRules _instance;

    if (!_initialised)
    {
        _initialised = true;
        _instance.set_default_options_values();
    }
    return _instance;
}

Adndtk::OptionalRules::OptionalRules()
{
}

Adndtk::OptionalRules::~OptionalRules()
{
}

void Adndtk::OptionalRules::set_default_options_values()
{
    _options[Option::max_score_for_hd] = false;
    _options[Option::scribe_scroll_always_succeeds] = false;
    _options[Option::encumbrance] = true;
    _options[Option::skills_generation_method] = static_cast<int>(SkillGenerationMethod::standard);
    _options[Option::max_starting_money] = false;
    _options[Option::enable_bonus_xp_for_high_prime_requisites] = true;
    _options[Option::check_for_deity_selection] = true;
    _options[Option::apply_aging_effect] = true;
    _options[Option::apply_encumbrance] = true;
    _options[Option::unlimited_store_availability] = false;
    _options[Option::unlimited_store_supply] = false;
    _options[Option::store_profit_margin_percentage] = 0.05;

    _options[Option::store_min_cp_starting_amount] = static_cast<uint32_t>(1000);
    _options[Option::store_max_cp_starting_amount] = static_cast<uint32_t>(10000);
    _options[Option::store_min_sp_starting_amount] = static_cast<uint32_t>(1000);
    _options[Option::store_max_sp_starting_amount] = static_cast<uint32_t>(5000);
    _options[Option::store_min_ep_starting_amount] = static_cast<uint32_t>(1000);
    _options[Option::store_max_ep_starting_amount] = static_cast<uint32_t>(2000);
    _options[Option::store_min_gp_starting_amount] = static_cast<uint32_t>(100);
    _options[Option::store_max_gp_starting_amount] = static_cast<uint32_t>(500);
    _options[Option::store_min_pp_starting_amount] = static_cast<uint32_t>(100);
    _options[Option::store_max_pp_starting_amount] = static_cast<uint32_t>(200);

    _options[Option::treasure_components_always_present] = false;
}