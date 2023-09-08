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
}