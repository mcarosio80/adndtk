#include <thaco.h>
#include <cyclopedia.h>
#include <dice.h>

Adndtk::Thaco::Thaco()
    : _clsType{}, _thacoScore{}, _thacoFactor{}
{
}

Adndtk::Thaco::Thaco(const Defs::character_class_type& clsType)
    : _clsType{clsType}, _thacoScore{}, _thacoFactor{}
{
    int typId = static_cast<int>(_clsType);
    auto thacoInfo = Cyclopedia::get_instance().exec_prepared_statement<int>(Query::select_thaco, typId);
    _thacoScore = thacoInfo[0].as<short>("score");
    _thacoFactor = thacoInfo[0].as<short>("factor");
}

Adndtk::Thaco::~Thaco()
{
}

Adndtk::THAC0 Adndtk::Thaco::get(const Adndtk::ExperienceLevel& lvl) const
{
    return 20 - (std::div(lvl-1, _thacoScore).quot) * _thacoFactor; 
}

Adndtk::Defs::attack_result Adndtk::Thaco::try_hit(const ExperienceLevel& lvl, const AC& ac, const short& bonusMalus/*=0*/) const
{
    Defs::attack_result res{Defs::attack_result::miss};

    Die d20{Defs::die::d20};
    auto roll = d20;
    auto thaco = get(lvl) - ac;

    if (roll == 1)
    {
        res = Defs::attack_result::critical_miss;
    }
    else if (roll == 20)
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