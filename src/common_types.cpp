#include <common_types.h>
#include <cyclopedia.h>

Adndtk::CharacterExperience::CharacterExperience(const Defs::character_class& cls)
{
    auto classes = Cyclopedia::get_instance().split(cls);
    for (auto& c : classes)
    {
        _xps[c] = std::make_pair(1, 0);
    }
}

void Adndtk::CharacterExperience::set(const Defs::character_class& cls, const ExperienceLevel& lvl, const XP& pts)
{
    if (_xps.find(cls) == _xps.end())
    {
        throw std::runtime_error("Invalid class specified");
    }
    _xps[cls] = std::make_pair(lvl, pts);
}