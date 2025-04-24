//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-MONS.001] Monsters have their relevant avatar type", "[monster]" )
{
    auto monsterId{Defs::monster::aarakocra};
    Monster m{monsterId};
    REQUIRE(m.get_avatar_type() == Avatar::Type::Monster);
}

TEST_CASE("[TC-MONS.002] Monsters basic features are retrieved as expected", "[monster]" )
{
    auto monsterId{Defs::monster::aarakocra};
    Monster m{monsterId};
    
    REQUIRE(m.get_id() == monsterId);
    REQUIRE(m.get_name() == "Aarakocra");
    REQUIRE(m.get_full_name() == "Aarakocra");

    REQUIRE(m.lives_in(Defs::climate::tropical));
    REQUIRE(m.lives_in(Defs::climate::temperate));
    REQUIRE(m.lives_in(Defs::terrain::mountains));
    REQUIRE(m.get_frequency().value() == Defs::encounter_frequency::very_rare);
    REQUIRE(m.lives_in(Defs::organisation::tribal));
    REQUIRE(m.active_at(Defs::activity_cycle::day));
    REQUIRE(m.eats(Defs::diet::carnivore));
    REQUIRE(m.get_intelligence() >= 8);
    REQUIRE(m.get_intelligence() <= 10);
    REQUIRE(m.moral_alignment().value() == Defs::moral_alignment::neutral_good);
    
    REQUIRE(m.get_planes().empty());
    REQUIRE_FALSE(m.is_undead());
    REQUIRE_FALSE(m.turned_as().has_value());
    REQUIRE_FALSE(m.is_elemental());
    REQUIRE_FALSE(m.element().has_value());
    REQUIRE_FALSE(m.is_dragon());
    REQUIRE_FALSE(m.dragon_type().has_value());
}