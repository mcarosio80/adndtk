#include <encumbrance.h>
#include <cyclopedia.h>
#include <options.h>
#include <skill_stats.h>

bool Adndtk::Encumbrance::_initialised = false;

Adndtk::Encumbrance& Adndtk::Encumbrance::get_instance()
{
    static Encumbrance _instance;

    if (!_initialised)
    {
        _initialised = true;
        _instance.load_base_movement_factors();
        _instance.load_modified_movement_rate();
    }
    return _instance;
}

Adndtk::Encumbrance::Encumbrance()
{
}

Adndtk::Encumbrance::~Encumbrance()
{
}

const short& Adndtk::Encumbrance::base_movement(const Defs::race& raceId) const
{
    return _baseMovFactors.at(raceId);
}

short Adndtk::Encumbrance::movement_factor(const SkillValue& strengthValue, const Defs::race& raceId, const double& weightAllowed) const
{
    short baseMovFactor = base_movement(raceId);

    if (!OptionalRules::get_instance().option<bool>(Option::encumbrance))
    {
        return baseMovFactor;
    }

    auto maxStrength = _movFactors.rbegin()->first;
    SkillValue strVal = strengthValue;
    if (strengthValue > maxStrength)
    {
        strVal = maxStrength;
    }

    auto mapWeightToMove = _movFactors.lower_bound(strVal)->second;
    auto maxAllowed = mapWeightToMove.rbegin()->first;
    auto stats = SkillStats::get_instance().get_strength_stats(strVal);
    if (weightAllowed > maxAllowed && weightAllowed <= stats.maximum_press)
    {
        return 1;
    }
    else if (weightAllowed > stats.maximum_press)
    {
        return 0;
    }

    auto movPair = mapWeightToMove.lower_bound(weightAllowed)->second;
    auto modifMovFactor = (baseMovFactor == Const::short_people_base_movement_factor) ? std::get<0>(movPair) : std::get<1>(movPair);

    return modifMovFactor;
}


void Adndtk::Encumbrance::load_base_movement_factors()
{
    auto movFactors = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_race_base_movement);
    for (auto& mf : movFactors)
    {
        auto raceId = mf.as<Defs::race>("id");
        auto baseMov = mf.as<short>("base_movement_rate");

        _baseMovFactors[raceId] = baseMov;
    }
}

void Adndtk::Encumbrance::load_modified_movement_rate()
{
    auto movRates = Cyclopedia::get_instance().exec_prepared_statement<>(Query::select_modified_movement_rate);
    for (auto& mr : movRates)
    {
        auto value = mr.as<short>("strength_to");
        auto excpt = mr.try_as<short>("exc_strength_to");
        auto load = mr.as<double>("load");
        auto mov1 = mr.as<short>("movement_rate_1");
        auto mov2 = mr.as<short>("movement_rate_2");
        SkillValue str{Defs::skill::strength, value, excpt};

        _movFactors[str][load] = std::make_pair(mov1, mov2);
    }
}