#include <thief_ability.h>
#include <cyclopedia.h>
#include <config.h>

#include <numeric>
#include <algorithm>

Adndtk::ThiefAbility::ThiefAbility(const Defs::race& r)
    : _race{r}, _level{1}, _improvementsAvailable{0}, _armourInUse{}, _dexterityValue{}
{
    set_base_abilities(_level);
    set_race_abilities(_race);
}

Adndtk::ThiefAbility::~ThiefAbility()
{
}

Adndtk::ThievingScore Adndtk::ThiefAbility::operator[] (const Defs::thief_ability& abilityId) const
{
    return std::min(95, _baseAbilities.at(abilityId)
            + _experienceAdjustments.at(abilityId)
            + _raceModifiers.at(abilityId)
            + _armourModifiers.at(abilityId)
            + _dexterityModifiers.at(abilityId));
}

void Adndtk::ThiefAbility::armour_change(const std::optional<Defs::equipment>& armourId/*=std::nullopt*/)
{
    if (armourId.has_value() && armourId != _armourInUse)
    {
        _armourInUse = armourId;
        set_armour_abilities(armourId.value());
    }
}

void Adndtk::ThiefAbility::dexterity_change(const short& skillValue)
{
    if (skillValue != _dexterityValue)
    {
        _dexterityValue = skillValue;
        set_dexterity_abilities(skillValue);
    }
}

void Adndtk::ThiefAbility::set_race_abilities(const Defs::race& race)
{
    short id = static_cast<short>(race);
    auto res = Cyclopedia::get_instance().exec_prepared_statement<short>(Query::select_thieving_skill_racial_adjustments, id);
    for (auto& r : res)
    {
        Defs::thief_ability thievingSkill = static_cast<Defs::thief_ability>(r.as<short>("thieving_skill"));
        short modifier = static_cast<short>(r.as<short>("modifier"));

        _raceModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_armour_abilities(const std::optional<Defs::equipment>& armourId)
{
    QueryResultSet res{};
    if (armourId.has_value())
    {
        short id = static_cast<short>(armourId.value());
        res = Cyclopedia::get_instance().exec_prepared_statement<short>(Query::select_thieving_skill_armour_adjustments, id);
    }
    else
    {
        res = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_thieving_skill_no_armour_adjustments);
    }

    for (auto& r : res)
    {
        Defs::thief_ability thievingSkill = static_cast<Defs::thief_ability>(r.as<short>("thieving_skill"));
        short modifier = static_cast<short>(r.as<short>("modifier"));

        _armourModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_dexterity_abilities(const short& skillValue)
{
    auto res = Cyclopedia::get_instance().exec_prepared_statement<short>(Query::select_thieving_skill_dexterity_adjustments, skillValue);
    for (auto& r : res)
    {
        Defs::thief_ability thievingSkill = static_cast<Defs::thief_ability>(r.as<short>("thieving_skill"));
        short modifier = static_cast<short>(r.as<short>("modifier"));

        _dexterityModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_base_abilities(const Adndtk::ExperienceLevel& lvl)
{
    short l = static_cast<short>(lvl);
    auto res = Cyclopedia::get_instance().exec_prepared_statement<short>(Query::select_thief_ability_scores, l);
    for (auto& r : res)
    {
        Defs::thief_ability ability = static_cast<Defs::thief_ability>(r.as<short>("thief_ability"));
        short score = static_cast<short>(r.as<short>("score"));

        _baseAbilities[ability] = score;
    }
}

void Adndtk::ThiefAbility::improve_abilities(const ThievingScore& pickPockets, const ThievingScore& openLocks,
                    const ThievingScore& findRemoveTraps, const ThievingScore& moveSilently,
                    const ThievingScore& hideInShadows, const ThievingScore& hearNoise,
                    const ThievingScore& climbWalls, const ThievingScore& readLanguages)
{
    std::vector<ThievingScore> scores;
    scores.reserve(8);
    scores[static_cast<short>(Defs::thief_ability::pick_pockets)] = pickPockets;
    scores[static_cast<short>(Defs::thief_ability::open_locks)] = openLocks;
    scores[static_cast<short>(Defs::thief_ability::find_remove_traps)] = findRemoveTraps;
    scores[static_cast<short>(Defs::thief_ability::move_silently)] = moveSilently;
    scores[static_cast<short>(Defs::thief_ability::hide_in_shadows)] = hideInShadows;
    scores[static_cast<short>(Defs::thief_ability::hear_noise)] = hearNoise;
    scores[static_cast<short>(Defs::thief_ability::climb_walls)] = climbWalls;
    scores[static_cast<short>(Defs::thief_ability::read_languages)] = readLanguages;

    improve_abilities(scores);
}

void Adndtk::ThiefAbility::improve_abilities(const std::vector<ThievingScore>& scores)
{
    if (_improvementsAvailable == 0)
    {
        ErrorManager::get_instance().error("Unable to improve skills");
        return;
    }

    ThievingScore maxTotal{};
    ThievingScore maxSingle{};
    if (_level = 1)
    {
        maxTotal = 60;
        maxSingle = 30;
    }
    else
    {
        maxTotal = 30;
        maxSingle = 15;
    }

    ThievingScore totalScore = std::accumulate(scores.begin(), scores.end(), 0);
    if (totalScore > maxTotal)
    {
        ErrorManager::get_instance().error("Maximum improvement exceeded");
        return;
    }
    
    ThievingScore maxScore = *std::max_element(scores.begin(), scores.end());
    if (maxScore > maxSingle)
    {
        ErrorManager::get_instance().error("Single value limit exceeded");
        return;
    }

    _improvementsAvailable--;
    for (int s=0; s<scores.size(); ++s)
    {
        auto sklId = static_cast<Defs::thief_ability>(s);
        _experienceAdjustments[sklId] = scores[s];
    }
}
