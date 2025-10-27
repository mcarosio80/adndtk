//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>

#include <defs.h>
#include <monster.h>

using namespace Adndtk;

TEST_CASE("[TC-MONS.001] Monsters have their relevant avatar type", "[monster]" )
{
    auto monsterId{Defs::monster::aarakocra};
    Monster m{monsterId};
    REQUIRE(m.avatar_type() == Avatar::Type::Monster);
}

TEST_CASE("[TC-MONS.002] Monsters basic features are retrieved as expected", "[monster]" )
{
    auto monsterId{Defs::monster::aarakocra};
    Monster m{monsterId};
    
    REQUIRE(m.get_id() == monsterId);
    REQUIRE(m.name() == "Aarakocra");
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
    Monster m{monsterId, Defs::monster_variant_type::priest};

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
    Monster m{monsterId, Defs::monster_variant_type::common};

    auto diet = m.get_diet();
    REQUIRE(diet.size() == 2);

    REQUIRE(m.eats(Defs::diet::carnivore));
    REQUIRE(m.eats(Defs::diet::man_eater));

    REQUIRE_FALSE(m.eats(Defs::diet::life_energy));
    REQUIRE_FALSE(m.eats(Defs::diet::herbivore));
}

TEST_CASE("[TC-MONS.008] Monsters with ratable intelligence have values between expected range", "[monster]" )
{
    Monster mindFlayer{Defs::monster::mind_flayer, Defs::monster_variant_type::common};
    REQUIRE(mindFlayer.get_intelligence() >= 17);
    REQUIRE(mindFlayer.get_intelligence() <= 18);

    Monster mimic{Defs::monster::mimic_killer_mimic, Defs::monster_variant_type::hd_10};
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

    Monster sahuagin{Defs::monster::sahuagin, Defs::monster_variant_type::common};
    REQUIRE(sahuagin.moral_alignment() == Defs::moral_alignment::lawful_evil);
}

TEST_CASE("[TC-MONS.011] Monsters with multiple moral alignments have one of the expected value", "[monster]" )
{
    Monster sylph{Defs::monster::elemental_air_kin_sylph};
    REQUIRE((sylph.moral_alignment() == Defs::moral_alignment::true_neutral
            || sylph.moral_alignment() == Defs::moral_alignment::neutral_good));
    
    Monster duergar{Defs::monster::dwarf_duergar, Defs::monster_variant_type::common};
    REQUIRE((duergar.moral_alignment() == Defs::moral_alignment::lawful_evil
            || duergar.moral_alignment() == Defs::moral_alignment::true_neutral));
    
    Monster kuotoa{Defs::monster::kuo_toa, Defs::monster_variant_type::normal};
    REQUIRE((kuotoa.moral_alignment() == Defs::moral_alignment::neutral_evil
            || kuotoa.moral_alignment() == Defs::moral_alignment::chaotic_evil));
}

TEST_CASE("[TC-MONS.012] Monsters with no moral alignment have no expected value", "[monster]" )
{
    Monster stirge{Defs::monster::stirge};
    REQUIRE_FALSE(stirge.moral_alignment().has_value());
}

TEST_CASE("[TC-MONS.013] Monsters with any moral alignment can have any value", "[monster]" )
{
    Monster birdMaiden{Defs::monster::swanmay_bird_maiden, Defs::monster_variant_type::hd_2};
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
    Monster m{Defs::monster::ankheg, Defs::monster_variant_type::hd_3};

    REQUIRE(m.ac_variants().size() == 2);
    REQUIRE(m.ac("overall").has_value());
    REQUIRE(m.ac("overall").value() == 2);
    REQUIRE(m.ac("underside").has_value());
    REQUIRE(m.ac("underside").value() == 4);

    REQUIRE(m.ac().value() == 2);

    REQUIRE_FALSE(m.ac("whatever else").has_value());
}

TEST_CASE("[TC-MONS.016] Monsters have HP scores according to their variant type", "[monster]" )
{
    Monster ankhegHd3{Defs::monster::ankheg, Defs::monster_variant_type::hd_3};
    REQUIRE(ankhegHd3.thac0() == 17);
    REQUIRE(ankhegHd3.hp() >= 3);
    REQUIRE(ankhegHd3.hp() <= 24);
    REQUIRE(ankhegHd3.xp() == 175);
    REQUIRE(ankhegHd3.variant() == Defs::monster_variant_type::hd_3);

    Monster ankhegHd5{Defs::monster::ankheg, Defs::monster_variant_type::hd_5};
    REQUIRE(ankhegHd5.thac0() == 15);
    REQUIRE(ankhegHd5.hp() >= 5);
    REQUIRE(ankhegHd5.hp() <= 40);
    REQUIRE(ankhegHd5.xp() == 420);
    REQUIRE(ankhegHd5.variant() == Defs::monster_variant_type::hd_5);

    Monster ankhegHd7{Defs::monster::ankheg, Defs::monster_variant_type::hd_7};
    REQUIRE(ankhegHd7.thac0() == 13);
    REQUIRE(ankhegHd7.hp() >= 7);
    REQUIRE(ankhegHd7.hp() <= 56);
    REQUIRE(ankhegHd7.xp() == 975);
    REQUIRE(ankhegHd7.variant() == Defs::monster_variant_type::hd_7);
}

TEST_CASE("[TC-MONS.017] Monsters with no variant specified are assigned the first variant available", "[monster]" )
{
    Monster mongrelman{Defs::monster::mongrelman};
    REQUIRE(mongrelman.thac0() == 19);
    REQUIRE(mongrelman.hp() >= 1);
    REQUIRE(mongrelman.hp() <= 8);
    REQUIRE(mongrelman.xp() == 35);
    REQUIRE(mongrelman.variant() == Defs::monster_variant_type::hd_1);

    Monster troglodyte{Defs::monster::troglodyte};
    REQUIRE(troglodyte.thac0() == 19);
    REQUIRE(troglodyte.hp() >= 2);
    REQUIRE(troglodyte.hp() <= 16);
    REQUIRE(troglodyte.xp() == 120);
    REQUIRE(troglodyte.variant() == Defs::monster_variant_type::normal);
}

TEST_CASE("[TC-MONS.018] Monsters can have unique names", "[monster]" )
{
    Monster lich{Defs::monster::lich};
    REQUIRE(lich.get_unique_name() == "");

    const char * lich_name{"Vecna"};
    Monster vecna{Defs::monster::lich, lich_name};
    REQUIRE(vecna.get_unique_name().compare(lich_name) == 0);
}