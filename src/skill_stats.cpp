#include <skill_stats.h>
#include <cyclopedia.h>


bool Adndtk::SkillStats::_initialised = false;



Adndtk::SkillStats::SkillStats()
{
}

Adndtk::SkillStats::~SkillStats()
{
}

Adndtk::SkillStats& Adndtk::SkillStats::get_instance()
{
    static SkillStats _instance;

    if (!_initialised)
    {
        _instance.init();
        _initialised = true;
    }
    return _instance;
}

const Adndtk::Tables::strength_stats& Adndtk::SkillStats::get_strength_stats(const SkillValue& skillValue) const
{
    return _strengthStats.upper_bound(skillValue)->second;
}

const Adndtk::Tables::dexterity_stats& Adndtk::SkillStats::get_dexterity_stats(const SkillValue& skillValue) const
{
    return _dexterityStats.at(skillValue);
}

const Adndtk::Tables::constitution_stats& Adndtk::SkillStats::get_constitution_stats(const SkillValue& skillValue) const
{
    return _constitutionStats.at(skillValue);
}

const Adndtk::Tables::intelligence_stats& Adndtk::SkillStats::get_intelligence_stats(const SkillValue& skillValue) const
{
    return _intelligenceStats.at(skillValue);
}

const Adndtk::Tables::wisdom_stats& Adndtk::SkillStats::get_wisdom_stats(const SkillValue& skillValue) const
{
    return _wisdomStats.at(skillValue);
}

const Adndtk::Tables::charisma_stats& Adndtk::SkillStats::get_charisma_stats(const SkillValue& skillValue) const
{
    return _charismaStats.at(skillValue);
}

void Adndtk::SkillStats::init()
{
    init_stats_for_strength();
    init_stats_for_dexterity();
    init_stats_for_constitution();
    init_stats_for_intelligence();
    init_stats_for_wisdom();
    init_stats_for_charisma();
}

void Adndtk::SkillStats::init_stats_for_strength()
{
    auto strStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_strength_stats);
    for (auto& r : strStats)
    {
        struct Tables::strength_stats stats;
        stats.id = r.try_as<int>("id").value();
        stats.ability_score_from = r.try_as<int>("ability_score_from").value();
        stats.excp_from = r.try_as<int>("excp_from");
        stats.ability_score_to = r.try_as<int>("").value();
        stats.excp_to = r.try_as<int>("excp_to");
        stats.hit_modifier = r.try_as<int>("hit_modifier").value();
        stats.damage_adjustment = r.try_as<int>("damage_adjustment").value();
        stats.weight_allowance = r.try_as<double>("weight_allowance").value();
        stats.maximum_press = r.try_as<double>("maximum_press").value();
        stats.open_doors = r.try_as<int>("open_doors").value();
        stats.open_doors_special = r.try_as<int>("open_doors_special");
        stats.bend_bars_lift_gates = r.try_as<int>("bend_bars_lift_gates").value();
        stats.note = r["note"];
        
        SkillValue skl{Defs::skill::strength, stats.ability_score_to, stats.excp_to};
        _strengthStats[skl] = stats;
    }
}

void Adndtk::SkillStats::init_stats_for_dexterity()
{
    auto dexStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_dexterity_stats);
    for (auto& r : dexStats)
    {
        struct Tables::dexterity_stats stats;
        stats.ability_score = r.try_as<int>("ability_score").value();
        stats.reaction_adjustment = r.try_as<int>("reaction_adjustment").value();
        stats.missile_attack_adjustment = r.try_as<int>("missile_attack_adjustment").value();
        stats.defensive_adjustment = r.try_as<int>("defensive_adjustment").value();

        SkillValue skl{Defs::skill::dexterity, stats.ability_score};
        _dexterityStats[skl] = stats;
    }
}

void Adndtk::SkillStats::init_stats_for_constitution()
{
    auto conStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_constitution_stats);
    for (auto& r : conStats)
    {
        struct Tables::constitution_stats stats;
        stats.ability_score = r.try_as<int>("ability_score").value();
        stats.hit_point_adjustment = r.try_as<int>("hit_point_adjustment").value();
        stats.hit_point_adjustment_warriors = r.try_as<int>("hit_point_adjustment_warriors");
        stats.rolls_upgrade = r.try_as<int>("rolls_upgrade");
        stats.system_shock = r.try_as<int>("system_shock").value();
        stats.resurrection_survival = r.try_as<int>("resurrection_survival").value();
        stats.poison_save = r.try_as<double>("poison_save").value();
        stats.regeneration_points = r.try_as<int>("regeneration_points");
        stats.regeneration_turns = r.try_as<int>("regeneration_turns");
       
        SkillValue skl{Defs::skill::constitution, stats.ability_score};
        _constitutionStats[skl] = stats;
    }
}

void Adndtk::SkillStats::init_stats_for_intelligence()
{
    auto intStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_intelligence_stats);
    for (auto& r : intStats)
    {
        struct Tables::intelligence_stats stats;
        stats.ability_score = r.try_as<int>("ability_score").value();
        stats.number_of_languages = r.try_as<int>("number_of_languages").value();
        stats.spell_level = r.try_as<int>("spell_level");
        stats.chance_to_learn_spell = r.try_as<int>("chance_to_learn_spell");
        stats.max_number_of_spells_per_level = r.try_as<int>("max_number_of_spells_per_level");
        stats.illusion_immunity = r.try_as<int>("illusion_immunity");

        SkillValue skl{Defs::skill::intelligence, stats.ability_score};
        _intelligenceStats[skl] = stats;
    }
}

void Adndtk::SkillStats::init_stats_for_wisdom()
{
    auto wisStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_wisdom_stats);
    for (auto& r : wisStats)
    {
        struct Tables::wisdom_stats stats;
        stats.ability_score = r.try_as<int>("ability_score").value();
        stats.magical_defence_adjustment = r.try_as<int>("magical_defence_adjustment").value();
        stats.bonus_spell_level_1 = r.try_as<int>("bonus_spell_level_1");
        stats.bonus_spell_level_2 = r.try_as<int>("bonus_spell_level_2");
        stats.bonus_spell_level_3 = r.try_as<int>("bonus_spell_level_3");
        stats.bonus_spell_level_4 = r.try_as<int>("bonus_spell_level_4");
        stats.bonus_spell_level_5 = r.try_as<int>("bonus_spell_level_5");
        stats.bonus_spell_level_6 = r.try_as<int>("bonus_spell_level_6");
        stats.bonus_spell_level_7 = r.try_as<int>("bonus_spell_level_7");
        stats.chance_of_spell_failure = r.try_as<int>("chance_of_spell_failure").value();
        stats.spell_immunity = r.try_as<int>("spell_immunity");

        SkillValue skl{Defs::skill::wisdom, stats.ability_score};
        _wisdomStats[skl] = stats;
    }
}

void Adndtk::SkillStats::init_stats_for_charisma()
{
    auto chaStats = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_charisma_stats);
    for (auto& r : chaStats)
    {
        struct Tables::charisma_stats stats;
        stats.ability_score = r.try_as<int>("ability_score").value();
        stats.maximum_number_of_henchmen = r.try_as<int>("maximum_number_of_henchmen").value();
        stats.loyalty_base = r.try_as<int>("loyalty_base").value();
        stats.reaction_adjustment = r.try_as<int>("reaction_adjustment");
        
        SkillValue skl{Defs::skill::charisma, stats.ability_score};
        _charismaStats[skl] = stats;
    }
}