#include <thaco.h>
#include <cyclopedia.h>

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

Adndtk::THAC0 Adndtk::Thaco::get(const Adndtk::ExperienceLevel& lvl)
{
    return 20 - (lvl - std::div(1, _thacoScore).quot) * _thacoFactor; 
}