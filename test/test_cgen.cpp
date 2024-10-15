//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include <optional>
#include <algorithm>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-CGEN.001] Human race is always available, no matter the skill values", "[character_generator]" )
{
    SkillValue str = SkillCreator::create(Defs::skill::strength, std::nullopt, std::nullopt);
    SkillValue dex = SkillCreator::create(Defs::skill::dexterity, std::nullopt, std::nullopt);
    SkillValue con = SkillCreator::create(Defs::skill::constitution, std::nullopt, std::nullopt);
    SkillValue inl = SkillCreator::create(Defs::skill::intelligence, std::nullopt, std::nullopt);
    SkillValue wis = SkillCreator::create(Defs::skill::wisdom, std::nullopt, std::nullopt);
    SkillValue cha = SkillCreator::create(Defs::skill::charisma, std::nullopt, std::nullopt);

    auto races = CharacterGenerator::available_races(str, dex, con, inl, wis, cha);
    auto pred = [](const Tables::race& r) -> bool
    {
        return static_cast<Defs::race>(r.id) == Defs::race::human;
    };
    auto found = std::find_if(races.begin(), races.end(), pred) != races.end();
    REQUIRE(found);
}