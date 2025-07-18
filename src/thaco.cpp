#include <thaco.h>
#include <cyclopedia.h>
#include <dice.h>

#include <cmath>

Adndtk::Thaco::Thaco()
{
}

Adndtk::Thaco::Thaco(const Defs::character_class_type& clsType)
{
    auto types = Cyclopedia::get_instance().split(clsType);

    for (auto& t : types)
    {
        auto thacoInfo = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class_type>(Query::select_thaco, t);
        auto thacoScore = thacoInfo[0].as<short>("score");
        auto thacoFactor = thacoInfo[0].as<short>("factor");

        _clsType.emplace(t);
        _thacoScore[t] = thacoScore;
        _thacoFactor[t] = thacoFactor;
    }
}

Adndtk::Thaco::~Thaco()
{
}

Adndtk::THAC0 Adndtk::Thaco::get(const Defs::character_class_type& clsType, const Adndtk::ExperienceLevel& lvl) const
{
    return Const::base_thaco - (std::div(lvl-1, _thacoScore.at(clsType)).quot) * _thacoFactor.at(clsType); 
}

Adndtk::THAC0 Adndtk::Thaco::get(const Adndtk::ExperienceLevel& lvl) const
{
    THAC0 bestThaco = Const::base_thaco;
    for (auto& t : _clsType)
    {
        auto thaco = get(t, lvl);
        if (thaco < bestThaco)
        {
            bestThaco = thaco;
        }
    }
    return bestThaco;
}

Adndtk::THAC0 Adndtk::Thaco::get_value(const HitDice& diceNumber, const short& diceModifier/*=0*/)
{
    if (diceNumber < 1.0 || (diceNumber == 1.0 && diceModifier < 0))
    {
        return Const::base_thaco;
    }

    auto thacoModifier = std::ceil(diceNumber / 2.0) - 1;
    return Const::base_thaco - 1 - (2 * thacoModifier);
}

std::pair<Adndtk::Defs::character_class_type, Adndtk::ExperienceLevel> Adndtk::Thaco::attack_as(const Adndtk::CharacterExperience& levels) const
{
    Defs::character_class selectedCls = levels.cbegin()->first;
    Defs::character_class_type selectedType = Cyclopedia::get_instance().get_class_type(selectedCls);
    ExperienceLevel selectedLevel = levels.level(selectedCls);

    THAC0 bestThaco = Const::base_thaco;
    for (auto it=levels.cbegin(); it!=levels.cend(); ++it)
    {
        auto type = Cyclopedia::get_instance().get_class_type(it->first);
        auto thaco = get(type, it->second.first);
        if (thaco < bestThaco)
        {
            bestThaco = thaco;
            selectedType = type;
            selectedLevel = levels.level(it->first);
        }
    }

    return std::make_pair(selectedType, selectedLevel);
}

Adndtk::Defs::attack_result Adndtk::Thaco::try_hit(const Adndtk::CharacterExperience& levels, const AC& ac, const short& bonusMalus/*=0*/) const
{
    Defs::attack_result res{Defs::attack_result::miss};
    auto selectet = attack_as(levels);

    Die d20{Defs::die::d20};
    auto roll = d20;
    auto thaco = get(selectet.first, selectet.second) - ac;

    if (roll == Const::critical_miss)
    {
        res = Defs::attack_result::critical_miss;
    }
    else if (roll == Const::critical_hit)
    {
        res = Defs::attack_result::critical_hit;
    }
    else if (roll + bonusMalus >= thaco)
    {
        res = Defs::attack_result::hit;
    }
    else
    {
        res = Defs::attack_result::miss;
    }

    return res;
}