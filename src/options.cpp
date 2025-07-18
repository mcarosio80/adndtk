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
    _options[Option::skills_generation_method] = static_cast<std::underlying_type_t<SkillGenerationMethod>>(SkillGenerationMethod::standard);
    _options[Option::max_starting_money] = false;
    _options[Option::enable_bonus_xp_for_high_prime_requisites] = true;
    _options[Option::check_for_deity_selection] = true;
    _options[Option::apply_aging_effect] = true;
    _options[Option::apply_encumbrance] = true;
    _options[Option::unlimited_store_availability] = false;
    _options[Option::unlimited_store_supply] = false;
    _options[Option::store_profit_margin_percentage] = Const::store_profit_margin_percentage;
    _options[Option::store_min_cp_starting_amount] = static_cast<uint32_t>(Const::store_min_value_cp);
    _options[Option::store_max_cp_starting_amount] = static_cast<uint32_t>(Const::store_max_value_cp);
    _options[Option::store_min_sp_starting_amount] = static_cast<uint32_t>(Const::store_min_value_sp);
    _options[Option::store_max_sp_starting_amount] = static_cast<uint32_t>(Const::store_max_value_sp);
    _options[Option::store_min_ep_starting_amount] = static_cast<uint32_t>(Const::store_min_value_ep);
    _options[Option::store_max_ep_starting_amount] = static_cast<uint32_t>(Const::store_max_value_ep);
    _options[Option::store_min_gp_starting_amount] = static_cast<uint32_t>(Const::store_min_value_gp);
    _options[Option::store_max_gp_starting_amount] = static_cast<uint32_t>(Const::store_max_value_gp);
    _options[Option::store_min_pp_starting_amount] = static_cast<uint32_t>(Const::store_min_value_pp);
    _options[Option::store_max_pp_starting_amount] = static_cast<uint32_t>(Const::store_max_value_pp);

    _options[Option::treasure_components_always_present] = false;
}