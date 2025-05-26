//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <defs.h>
#include <monster.h>

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

TEST_CASE("[TC-MONS.003] Monsters with multiple climate/cerrain can be found as expected", "[monster]" )
{
    auto monsterId{Defs::monster::aboleth};
    Monster m{monsterId};

    auto climateTerrain = m.get_climate_terrain();
    REQUIRE(climateTerrain.size() == 2);

    REQUIRE(m.lives_in(Defs::climate::tropical));
    REQUIRE(m.lives_in(Defs::climate::temperate));
    REQUIRE(m.lives_in(Defs::terrain::subterranean));

    REQUIRE_FALSE(m.lives_in(Defs::climate::arctic));
    REQUIRE_FALSE(m.lives_in(Defs::climate::space));
    REQUIRE_FALSE(m.lives_in(Defs::climate::dry));
    REQUIRE_FALSE(m.lives_in(Defs::climate::arid));
    REQUIRE_FALSE(m.lives_in(Defs::climate::any));
    REQUIRE_FALSE(m.lives_in(Defs::terrain::lakes));
    REQUIRE_FALSE(m.lives_in(Defs::terrain::sea));
    REQUIRE_FALSE(m.lives_in(Defs::terrain::freshwater));
    REQUIRE_FALSE(m.lives_in(Defs::terrain::saltwater));
    REQUIRE_FALSE(m.lives_in(Defs::terrain::lakes));
}

TEST_CASE("[TC-MONS.004] Monsters with multiple organisation can be found as expected", "[monster]" )
{
    auto monsterId{Defs::monster::elf_drow};
    Monster m{monsterId};

    auto organisation = m.get_organisation();
    REQUIRE(organisation.size() == 2);

    REQUIRE(m.lives_in(Defs::organisation::clan));
    REQUIRE(m.lives_in(Defs::organisation::band));

    REQUIRE_FALSE(m.lives_in(Defs::organisation::platoon));
    REQUIRE_FALSE(m.lives_in(Defs::organisation::tribal));
    REQUIRE_FALSE(m.lives_in(Defs::organisation::family));
}

TEST_CASE("[TC-MONS.005] Monsters with activity cycle can be found as expected", "[monster]" )
{
    auto monsterId{Defs::monster::dracolich};
    Monster m{monsterId};

    auto activityCycle = m.get_activity_cycle();
    REQUIRE(activityCycle.size() == 1);

    REQUIRE(m.active_at(Defs::activity_cycle::night));
    REQUIRE_FALSE(m.active_at(Defs::activity_cycle::day));
    REQUIRE_FALSE(m.active_at(Defs::activity_cycle::hottest_part_of_the_day));
    REQUIRE_FALSE(m.active_at(Defs::activity_cycle::early_morning));
}

TEST_CASE("[TC-MONS.006] Monsters with no diet eats nothing", "[monster]" )
{
    auto monsterId{Defs::monster::revenant};
    Monster m{monsterId};

    auto diet = m.get_diet();
    REQUIRE(diet.size() == 0);
}

TEST_CASE("[TC-MONS.007] Monsters with varied diet can eat many things", "[monster]" )
{
    auto monsterId{Defs::monster::minotaur};
    Monster m{monsterId};

    auto diet = m.get_diet();
    REQUIRE(diet.size() == 2);

    REQUIRE(m.eats(Defs::diet::carnivore));
    REQUIRE(m.eats(Defs::diet::man_eater));

    REQUIRE_FALSE(m.eats(Defs::diet::life_energy));
    REQUIRE_FALSE(m.eats(Defs::diet::herbivore));
}

TEST_CASE("[TC-MONS.008] Monsters with ratable intelligence have values between expected range", "[monster]" )
{
    Monster mindFlayer{Defs::monster::mind_flayer};
    REQUIRE(mindFlayer.get_intelligence() >= 17);
    REQUIRE(mindFlayer.get_intelligence() <= 18);

    Monster mimic{Defs::monster::mimic_killer_mimic};
    REQUIRE(mimic.get_intelligence() >= 2);
    REQUIRE(mimic.get_intelligence() <= 4);

    Monster haunt{Defs::monster::haunt};
    REQUIRE(haunt.get_intelligence() == 0);
}

TEST_CASE("[TC-MONS.009] Monsters with non-ratable intelligence have no intelligence value", "[monster]" )
{
    Monster m{Defs::monster::intellect_devourer_larva};
    REQUIRE_FALSE(m.get_intelligence().has_value());
}

TEST_CASE("[TC-MONS.010] Monsters with moral alignment have value as expected", "[monster]" )
{
    Monster werewolf{Defs::monster::lycanthrope_werewolf};
    REQUIRE(werewolf.moral_alignment() == Defs::moral_alignment::chaotic_evil);

    Monster phantom{Defs::monster::phantom};
    REQUIRE(phantom.moral_alignment() == Defs::moral_alignment::true_neutral);

    Monster sahuagin{Defs::monster::sahuagin};
    REQUIRE(sahuagin.moral_alignment() == Defs::moral_alignment::lawful_evil);
}

TEST_CASE("[TC-MONS.011] Monsters with multiple moral alignments have one of the expected value", "[monster]" )
{
    Monster sylph{Defs::monster::elemental_air_kin_sylph};
    REQUIRE((sylph.moral_alignment() == Defs::moral_alignment::true_neutral
            || sylph.moral_alignment() == Defs::moral_alignment::neutral_good));
    
    Monster duergar{Defs::monster::dwarf_duergar};
    REQUIRE((duergar.moral_alignment() == Defs::moral_alignment::lawful_evil
            || duergar.moral_alignment() == Defs::moral_alignment::true_neutral));
    
    Monster kuotoa{Defs::monster::kuo_toa};
    REQUIRE((kuotoa.moral_alignment() == Defs::moral_alignment::neutral_evil
            || kuotoa.moral_alignment() == Defs::moral_alignment::chaotic_evil));
}

TEST_CASE("[TC-MONS.012] Monsters with no moral alignment have no expected value", "[monster]" )
{
    Monster sylph{Defs::monster::stirge};
    REQUIRE_FALSE(sylph.moral_alignment().has_value());
}

TEST_CASE("[TC-MONS.013] Monsters with any moral alignment can have any value", "[monster]" )
{
    Monster birdMaiden{Defs::monster::swanmay_bird_maiden};
    REQUIRE((birdMaiden.moral_alignment() == Defs::moral_alignment::lawful_good
        || birdMaiden.moral_alignment() == Defs::moral_alignment::lawful_neutral
        || birdMaiden.moral_alignment() == Defs::moral_alignment::lawful_evil
        || birdMaiden.moral_alignment() == Defs::moral_alignment::neutral_good
        || birdMaiden.moral_alignment() == Defs::moral_alignment::true_neutral
        || birdMaiden.moral_alignment() == Defs::moral_alignment::neutral_evil
        || birdMaiden.moral_alignment() == Defs::moral_alignment::chaotic_good
        || birdMaiden.moral_alignment() == Defs::moral_alignment::chaotic_neutral
        || birdMaiden.moral_alignment() == Defs::moral_alignment::chaotic_evil));

    Monster lich{Defs::monster::lich};
    REQUIRE((lich.moral_alignment() == Defs::moral_alignment::lawful_evil
        || lich.moral_alignment() == Defs::moral_alignment::neutral_evil
        || lich.moral_alignment() == Defs::moral_alignment::chaotic_evil));

    Monster nereid{Defs::monster::elemental_water_kin_nereid};
    REQUIRE((nereid.moral_alignment() == Defs::moral_alignment::chaotic_good
        || nereid.moral_alignment() == Defs::moral_alignment::chaotic_neutral
        || nereid.moral_alignment() == Defs::moral_alignment::chaotic_evil));
}

TEST_CASE("[TC-MONS.014] Monsters with single AC value have no AC variants", "[monster]" )
{
    Monster m{Defs::monster::golem_gargoyle};

    REQUIRE(m.ac_variants().size() == 1);
    REQUIRE(m.ac().has_value());
    REQUIRE(m.ac().value() == 0);
}

TEST_CASE("[TC-MONS.015] Monsters with multiple AC value have AC variants", "[monster]" )
{
    Monster m{Defs::monster::ankheg};

    REQUIRE(m.ac_variants().size() == 2);
    REQUIRE(m.ac("overall").has_value());
    REQUIRE(m.ac("overall").value() == 2);
    REQUIRE(m.ac("underside").has_value());
    REQUIRE(m.ac("underside").value() == 4);

    REQUIRE(m.ac().value() == 2);

    REQUIRE_FALSE(m.ac("whatever else").has_value());
}