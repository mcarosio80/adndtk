#include <thief_ability.h>
#include <cyclopedia.h>
#include <config.h>
#include <adnd_errors.h>

#include <numeric>
#include <algorithm>

Adndtk::ThiefAbility::ThiefAbility(const Defs::race& r)
    : _race{r}, _level{1}, _improvementsAvailable{1}, _armourInUse{std::nullopt}, _dexterityValue{9}
{
    for (auto thSkl : { Defs::thief_ability::pick_pockets, Defs::thief_ability::open_locks,
                        Defs::thief_ability::find_remove_traps, Defs::thief_ability::move_silently,
                        Defs::thief_ability::hide_in_shadows, Defs::thief_ability::hear_noise,
                        Defs::thief_ability::climb_walls, Defs::thief_ability::read_languages })
    {
        _baseAbilities[thSkl] = 0;
        _raceModifiers[thSkl] = 0;
        _experienceAdjustments[thSkl] = {};
        _armourModifiers[thSkl] = 0;
        _dexterityModifiers[thSkl] = 0; 
    }
    set_base_abilities();
    set_race_abilities(_race);
    set_dexterity_abilities(_dexterityValue);
    set_armour_abilities(_armourInUse);
}

Adndtk::ThiefAbility::~ThiefAbility()
{
}

short Adndtk::ThiefAbility::available_ability_slots() const
{
    return std::max<short>(0, _level - _experienceAdjustments.at(Defs::thief_ability::pick_pockets).size());
}

Adndtk::ThievingScore Adndtk::ThiefAbility::operator[] (const Defs::thief_ability& abilityId) const
{
    if (_dexterityValue < 9)
        return 0;

    return std::min(95, _baseAbilities.at(abilityId)
            + experience_adjustment(abilityId)
            + _raceModifiers.at(abilityId)
            + _armourModifiers.at(abilityId)
            + _dexterityModifiers.at(abilityId));
}

Adndtk::ThievingScore Adndtk::ThiefAbility::get(const Defs::thief_ability& abilityId, const short& bonusMalus) const
{
    if (_dexterityValue < 9)
        return 0;

    return std::min(95, _baseAbilities.at(abilityId)
            + experience_adjustment(abilityId)
            + _raceModifiers.at(abilityId)
            + _armourModifiers.at(abilityId)
            + _dexterityModifiers.at(abilityId)
            + bonusMalus);
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

short Adndtk::ThiefAbility::level_change(const ExperienceLevel& newLevel)
{
    auto sklLen = _experienceAdjustments[Defs::thief_ability::pick_pockets].size();
    _level = newLevel;
    if (newLevel == sklLen)
    {
        return 0;
    }
    
    _improvementsAvailable = newLevel - sklLen;
    return std::max<short>(_level - sklLen, 0);
}

void Adndtk::ThiefAbility::set_race_abilities(const Defs::race& race)
{
    auto res = Cyclopedia::get_instance().exec_prepared_statement<Defs::race>(Query::select_thieving_skill_racial_adjustments, race);
    for (auto& r : res)
    {
        auto thievingSkill = r.as<Defs::thief_ability>("thieving_skill");
        auto modifier = r.try_or<short>("modifier", 0);
        _raceModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_armour_abilities(const std::optional<Defs::equipment>& armourId)
{
    QueryResultSet res{};
    if (armourId.has_value())
    {
        res = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_thieving_skill_armour_adjustments, armourId.value());
    }
    else
    {
        res = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_thieving_skill_no_armour_adjustments);
    }

    for (auto& r : res)
    {
        auto thievingSkill = r.as<Defs::thief_ability>("thieving_skill");
        auto modifier = r.try_or<short>("modifier", 0);

        _armourModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_dexterity_abilities(const short& skillValue)
{
    auto res = Cyclopedia::get_instance().exec_prepared_statement<short>(Query::select_thieving_skill_dexterity_adjustments, skillValue);
    for (auto& r : res)
    {
        Defs::thief_ability thievingSkill = r.as<Defs::thief_ability>("thieving_skill");
        auto modifier = r.try_or<short>("modifier", 0);
        _dexterityModifiers[thievingSkill] = modifier;
    }
}

void Adndtk::ThiefAbility::set_base_abilities()
{
    auto res = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_thief_ability_base_scores);
    for (auto& r : res)
    {
        auto ability = r.as<Defs::thief_ability>("id");
        auto score = r.as<short>("base_score");

        _baseAbilities[ability] = score;
    }
}

void Adndtk::ThiefAbility::improve_abilities(const ThievingScore& pickPockets, const ThievingScore& openLocks,
                    const ThievingScore& findRemoveTraps, const ThievingScore& moveSilently,
                    const ThievingScore& hideInShadows, const ThievingScore& hearNoise,
                    const ThievingScore& climbWalls, const ThievingScore& readLanguages)
{
    std::vector<ThievingScore> scores(8);
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
    if (_improvementsAvailable <= 0)
    {
        throw SkillException("Unable to improve skills");
    }

    increase_abilities(scores);
}

Adndtk::ThievingScore Adndtk::ThiefAbility::experience_adjustment(const Defs::thief_ability& abilityId) const
{
    ThievingScore ts{0};
    auto limit = std::min<short>(_experienceAdjustments.at(abilityId).size(), _level);
    for (ExperienceLevel lvl = 0; lvl < limit; ++lvl)
    {
        ts += _experienceAdjustments.at(abilityId)[lvl];
    }
    return ts;
}

void Adndtk::ThiefAbility::increase_abilities(const std::vector<ThievingScore>& scores)
{
    ThievingScore maxTotal{};
    ThievingScore maxSingle{};

    auto sklLevel = static_cast<short>(_experienceAdjustments[Defs::thief_ability::pick_pockets].size());
    if (sklLevel == 0)
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
        throw SkillException("Maximum improvement exceeded");
    }
    
    ThievingScore maxScore = *std::max_element(scores.begin(), scores.end());
    if (maxScore > maxSingle)
    {
        throw SkillException("Single value limit exceeded");
    }

    _improvementsAvailable--;
    for (int s=0; s<scores.size(); ++s)
    {
        auto sklId = static_cast<Defs::thief_ability>(s);
        if (_experienceAdjustments[sklId].size() < _level)
        {
            _experienceAdjustments[sklId].push_back(scores[s]);
        }
    }
}