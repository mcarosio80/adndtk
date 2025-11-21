//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <holy_symbol.h>
#include <adnd_errors.h>

using namespace Adndtk;

TEST_CASE("[TC-HOLS.001] Low level caster cannot access higher level spells", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};

    REQUIRE_THROWS_AS(hs[Defs::priest_spell::cure_serious_wounds], SpellException<Defs::priest_spell>);
}

TEST_CASE("[TC-HOLS.002] Clerics gain spells automatically with new levels", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};

	REQUIRE(hs[Defs::priest_spell::animal_friendship] == 0);
	REQUIRE(hs[Defs::priest_spell::bless] == 0);
	REQUIRE(hs[Defs::priest_spell::combine] == 0);
	REQUIRE(hs[Defs::priest_spell::command] == 0);
	REQUIRE(hs[Defs::priest_spell::create_water] == 0);
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_evil] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_magic] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_poison] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_snares_and_pits] == 0);
	REQUIRE(hs[Defs::priest_spell::endure_heat_endure_cold] == 0);
	REQUIRE(hs[Defs::priest_spell::entangle] == 0);
	REQUIRE(hs[Defs::priest_spell::faerie_fire] == 0);
	REQUIRE(hs[Defs::priest_spell::invisibility_to_animals] == 0);
	REQUIRE(hs[Defs::priest_spell::invisibility_to_undead] == 0);
	REQUIRE(hs[Defs::priest_spell::light] == 0);
	REQUIRE(hs[Defs::priest_spell::locate_animals_or_plants] == 0);
	REQUIRE(hs[Defs::priest_spell::magical_stone] == 0);
	REQUIRE(hs[Defs::priest_spell::pass_without_trace] == 0);
	REQUIRE(hs[Defs::priest_spell::protection_from_evil] == 0);
	REQUIRE(hs[Defs::priest_spell::purify_food_and_drink] == 0);
	REQUIRE(hs[Defs::priest_spell::remove_fear] == 0);
	REQUIRE(hs[Defs::priest_spell::sanctuary] == 0);
	REQUIRE(hs[Defs::priest_spell::shillelagh] == 0);

    REQUIRE_THROWS_AS(hs[Defs::priest_spell::aid], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::augury], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::barkskin], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::chant], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::charm_person_or_mammal], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::detect_charm], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::dust_devil], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::enthrall], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::find_traps], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::fire_trap], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::flame_blade], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::goodberry], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::heat_metal], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::hold_person], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::know_alignment], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::messenger], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::obscurement], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::produce_flame], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::resist_fire_resist_cold], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::silence_15_radius], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::slow_poison], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::snake_charm ], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::speak_with_animals], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::spiritual_hammer], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::trip], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::warp_wood], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::withdraw], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::wyvern_watch], SpellException<Defs::priest_spell>);

    hs.set_caster_level(3);
    REQUIRE(hs[Defs::priest_spell::aid] == 0);
    REQUIRE(hs[Defs::priest_spell::augury] == 0);
    REQUIRE(hs[Defs::priest_spell::barkskin] == 0);
    REQUIRE(hs[Defs::priest_spell::chant] == 0);
    REQUIRE(hs[Defs::priest_spell::charm_person_or_mammal] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_charm] == 0);
    REQUIRE(hs[Defs::priest_spell::dust_devil] == 0);
    REQUIRE(hs[Defs::priest_spell::enthrall] == 0);
    REQUIRE(hs[Defs::priest_spell::find_traps] == 0);
    REQUIRE(hs[Defs::priest_spell::fire_trap] == 0);
    REQUIRE(hs[Defs::priest_spell::flame_blade] == 0);
    REQUIRE(hs[Defs::priest_spell::goodberry] == 0);
    REQUIRE(hs[Defs::priest_spell::heat_metal] == 0);
    REQUIRE(hs[Defs::priest_spell::hold_person] == 0);
    REQUIRE(hs[Defs::priest_spell::know_alignment] == 0);
    REQUIRE(hs[Defs::priest_spell::messenger] == 0);
    REQUIRE(hs[Defs::priest_spell::obscurement] == 0);
    REQUIRE(hs[Defs::priest_spell::produce_flame] == 0);
    REQUIRE(hs[Defs::priest_spell::resist_fire_resist_cold] == 0);
    REQUIRE(hs[Defs::priest_spell::silence_15_radius] == 0);
    REQUIRE(hs[Defs::priest_spell::slow_poison] == 0);
    REQUIRE(hs[Defs::priest_spell::snake_charm ] == 0);
    REQUIRE(hs[Defs::priest_spell::speak_with_animals] == 0);
    REQUIRE(hs[Defs::priest_spell::spiritual_hammer] == 0);
    REQUIRE(hs[Defs::priest_spell::trip] == 0);
    REQUIRE(hs[Defs::priest_spell::warp_wood] == 0);
    REQUIRE(hs[Defs::priest_spell::withdraw] == 0);
    REQUIRE(hs[Defs::priest_spell::wyvern_watch] == 0);
}

TEST_CASE("[TC-HOLS.003] Priests of specific mythos gain spells according to the spheres of influence connected to their god", "[spells, holy_symbol]" )
{
    auto deityId = Defs::deity::lathander;
    HolySymbol hs{Defs::character_class::preist_of_specific_mythos, deityId};
    hs.set_caster_level(3);

    REQUIRE(hs[Defs::priest_spell::flame_blade] == 0);
    REQUIRE(hs[Defs::priest_spell::goodberry] == 0);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::messenger], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::resist_fire_resist_cold], SpellException<Defs::priest_spell>);
}

TEST_CASE("[TC-HOLS.004] Clerics gain all spells available regardless to the spheres of influence", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(3);

    REQUIRE(hs[Defs::priest_spell::flame_blade] == 0);
    REQUIRE(hs[Defs::priest_spell::goodberry] == 0);
    REQUIRE(hs[Defs::priest_spell::messenger] == 0);
    REQUIRE(hs[Defs::priest_spell::resist_fire_resist_cold] == 0);
}

TEST_CASE("[TC-HOLS.005] Druids gain all spells available to their spheres of influence", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::druid};

    REQUIRE(hs[Defs::priest_spell::animal_friendship] == 0);
    REQUIRE(hs[Defs::priest_spell::bless] == 0);
    REQUIRE(hs[Defs::priest_spell::combine] == 0);
    REQUIRE(hs[Defs::priest_spell::create_water] == 0);
    REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_evil] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_magic] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_poison] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_snares_and_pits] == 0);
    REQUIRE(hs[Defs::priest_spell::entangle] == 0);
    REQUIRE(hs[Defs::priest_spell::faerie_fire] == 0);
    REQUIRE(hs[Defs::priest_spell::invisibility_to_animals] == 0);
    REQUIRE(hs[Defs::priest_spell::locate_animals_or_plants] == 0);
    REQUIRE(hs[Defs::priest_spell::pass_without_trace] == 0);
    REQUIRE(hs[Defs::priest_spell::purify_food_and_drink] == 0);
    REQUIRE(hs[Defs::priest_spell::shillelagh] == 0);

    REQUIRE_THROWS_AS(hs[Defs::priest_spell::command], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::endure_heat_endure_cold], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::invisibility_to_undead], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::light], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::magical_stone], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::protection_from_evil], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::remove_fear], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::sanctuary], SpellException<Defs::priest_spell>);
}

TEST_CASE("[TC-HOLS.006] Paladins cast spells like a priest when reach 9th level", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::paladin};

    auto spellId = Defs::priest_spell::cure_light_wounds;
    hs.set_caster_level(8);
    REQUIRE_THROWS_AS(hs[spellId], SpellException<Defs::priest_spell>);

    hs.set_caster_level(9);
    REQUIRE(hs[spellId] == 0);
    REQUIRE(hs.free_slots(1) == 1);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 1);

    REQUIRE(hs.pray_for_spell(spellId));
    REQUIRE(hs[spellId] == 1);
    REQUIRE(hs.free_slots(1) == 0);
    REQUIRE(hs.used_slots(1) == 1);
    REQUIRE(hs.total_slots(1) == 1);

    REQUIRE(hs.remove(spellId));
    REQUIRE(hs[spellId] == 0);
    REQUIRE(hs.free_slots(1) == 1);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 1);
}

TEST_CASE("[TC-HOLS.007] Paladins cast spells available to their spheres of influence", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::paladin};

    hs.set_caster_level(9);
    REQUIRE(hs[Defs::priest_spell::bless] == 0);
    REQUIRE(hs[Defs::priest_spell::combine] == 0);
    REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_evil] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_magic] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_poison] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_snares_and_pits] == 0);
    REQUIRE(hs[Defs::priest_spell::endure_heat_endure_cold] == 0);
    REQUIRE(hs[Defs::priest_spell::locate_animals_or_plants] == 0);
    REQUIRE(hs[Defs::priest_spell::magical_stone] == 0);
    REQUIRE(hs[Defs::priest_spell::protection_from_evil] == 0);
    REQUIRE(hs[Defs::priest_spell::purify_food_and_drink] == 0);
    REQUIRE(hs[Defs::priest_spell::sanctuary] == 0);
    REQUIRE(hs[Defs::priest_spell::shillelagh] == 0);
    
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::entangle], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::light], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::pass_without_trace], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::remove_fear], SpellException<Defs::priest_spell>);
}

TEST_CASE("[TC-HOLS.008] Paladins have no bonus spells for higher wisdom score", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::paladin};
    hs.set_caster_level(10);
    hs.set_caster_wisdom(18);

    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 2);
}

TEST_CASE("[TC-HOLS.009] Rangers cast spells like a priest when reach 8th level", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::ranger};

    auto spellId = Defs::priest_spell::detect_evil;
    hs.set_caster_level(7);
    REQUIRE_THROWS_AS(hs[spellId], SpellException<Defs::priest_spell>);

    hs.set_caster_level(8);
    REQUIRE(hs[spellId] == 0);
    REQUIRE(hs.free_slots(1) == 1);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 1);

    REQUIRE(hs.pray_for_spell(spellId));
    REQUIRE(hs[spellId] == 1);
    REQUIRE(hs.free_slots(1) == 0);
    REQUIRE(hs.used_slots(1) == 1);
    REQUIRE(hs.total_slots(1) == 1);

    REQUIRE(hs.remove(spellId));
    REQUIRE(hs[spellId] == 0);
    REQUIRE(hs.free_slots(1) == 1);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 1);
}

TEST_CASE("[TC-HOLS.010] Rangers cast spells available to their spheres of influence", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::ranger};

    hs.set_caster_level(8);

    REQUIRE(hs[Defs::priest_spell::animal_friendship] == 0);
    REQUIRE(hs[Defs::priest_spell::bless] == 0);
    REQUIRE(hs[Defs::priest_spell::combine] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_evil] == 0);
    REQUIRE(hs[Defs::priest_spell::entangle] == 0);
    REQUIRE(hs[Defs::priest_spell::invisibility_to_animals] == 0);
    REQUIRE(hs[Defs::priest_spell::locate_animals_or_plants] == 0);
    REQUIRE(hs[Defs::priest_spell::pass_without_trace] == 0);
    REQUIRE(hs[Defs::priest_spell::purify_food_and_drink] == 0);
    REQUIRE(hs[Defs::priest_spell::shillelagh] == 0);
    
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::command], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::create_water], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::detect_poison], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::invisibility_to_undead], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::remove_fear], SpellException<Defs::priest_spell>);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::sanctuary], SpellException<Defs::priest_spell>);
}

TEST_CASE("[TC-HOLS.011] Rangers have no bonus spells for higher wisdom score", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::ranger};
    hs.set_caster_level(10);
    hs.set_caster_wisdom(18);

    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.free_slots(2) == 1);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.total_slots(2) == 1);
}

TEST_CASE("[TC-HOLS.012] Casters cannot exceed the number of spells available", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(2);

	REQUIRE(hs[Defs::priest_spell::animal_friendship] == 0);
	REQUIRE(hs[Defs::priest_spell::bless] == 0);
	REQUIRE(hs[Defs::priest_spell::combine] == 0);
	REQUIRE(hs[Defs::priest_spell::command] == 0);
	REQUIRE(hs[Defs::priest_spell::create_water] == 0);
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_evil] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_magic] == 0);
    REQUIRE(hs[Defs::priest_spell::detect_poison] == 0);
	REQUIRE(hs[Defs::priest_spell::detect_snares_and_pits] == 0);
	REQUIRE(hs[Defs::priest_spell::endure_heat_endure_cold] == 0);
	REQUIRE(hs[Defs::priest_spell::entangle] == 0);
	REQUIRE(hs[Defs::priest_spell::faerie_fire] == 0);
	REQUIRE(hs[Defs::priest_spell::invisibility_to_animals] == 0);
	REQUIRE(hs[Defs::priest_spell::invisibility_to_undead] == 0);
	REQUIRE(hs[Defs::priest_spell::light] == 0);
	REQUIRE(hs[Defs::priest_spell::locate_animals_or_plants] == 0);
	REQUIRE(hs[Defs::priest_spell::magical_stone] == 0);
	REQUIRE(hs[Defs::priest_spell::pass_without_trace] == 0);
	REQUIRE(hs[Defs::priest_spell::protection_from_evil] == 0);
	REQUIRE(hs[Defs::priest_spell::purify_food_and_drink] == 0);
	REQUIRE(hs[Defs::priest_spell::remove_fear] == 0);
	REQUIRE(hs[Defs::priest_spell::sanctuary] == 0);
	REQUIRE(hs[Defs::priest_spell::shillelagh] == 0);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 1);
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 2);
    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::protection_from_evil));
}

TEST_CASE("[TC-HOLS.013] Praying for spells consume available slots", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};

    REQUIRE(hs.total_slots(1) == 1);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 1);

    hs.set_caster_level(2);

    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 1);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 1);
    REQUIRE(hs.free_slots(1) == 1);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 2);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 2);
    REQUIRE(hs.free_slots(1) == 0);

    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 2);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 2);
    REQUIRE(hs.free_slots(1) == 0);

    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::entangle));
	REQUIRE(hs[Defs::priest_spell::entangle] == 0);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 2);
    REQUIRE(hs.free_slots(1) == 0);
}

TEST_CASE("[TC-HOLS.014] Removing spells free up available slots", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(2);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 2);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 2);
    REQUIRE(hs.free_slots(1) == 0);

    REQUIRE(hs.remove(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 1);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 1);
    REQUIRE(hs.free_slots(1) == 1);

    REQUIRE(hs.remove(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);

    REQUIRE_FALSE(hs.remove(Defs::priest_spell::cure_light_wounds));
	REQUIRE(hs[Defs::priest_spell::cure_light_wounds] == 0);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);
}

TEST_CASE("[TC-HOLS.015] Increasing the caster level enable access to higher level spells", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::cure_critical_wounds));

    hs.set_caster_level(9);
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_critical_wounds));
}

TEST_CASE("[TC-HOLS.016] Reducing the caster level disable access to higher level spells", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(3);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.total_slots(2) == 1);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.free_slots(2) == 1);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::flame_blade));
    REQUIRE(hs[Defs::priest_spell::flame_blade] == 1);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.total_slots(2) == 1);
    REQUIRE(hs.used_slots(2) == 1);
    REQUIRE(hs.free_slots(2) == 0);

    hs.set_caster_level(2);
    REQUIRE_THROWS_AS(hs[Defs::priest_spell::flame_blade], SpellException<Defs::priest_spell>);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.total_slots(2) == 0);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.free_slots(2) == 0);

    hs.set_caster_level(3);
    REQUIRE(hs[Defs::priest_spell::flame_blade] == 0);
    REQUIRE(hs.total_slots(1) == 2);
    REQUIRE(hs.used_slots(1) == 0);
    REQUIRE(hs.free_slots(1) == 2);
    REQUIRE(hs.total_slots(2) == 1);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.free_slots(2) == 1);
}

TEST_CASE("[TC-HOLS.017] Priests with higher wisdom scores have bonus spell slots", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(9);
    
    REQUIRE(hs.total_slots(1) == 4);
    REQUIRE(hs.total_slots(2) == 4);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(13);
    REQUIRE(hs.total_slots(1) == 5);
    REQUIRE(hs.total_slots(2) == 4);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(14);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 4);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(15);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 5);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(16);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 6);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(17);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 6);
    REQUIRE(hs.total_slots(3) == 4);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(18);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 6);
    REQUIRE(hs.total_slots(3) == 4);
    REQUIRE(hs.total_slots(4) == 3);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(19);
    REQUIRE(hs.total_slots(1) == 7);
    REQUIRE(hs.total_slots(2) == 6);
    REQUIRE(hs.total_slots(3) == 5);
    REQUIRE(hs.total_slots(4) == 3);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(20);
    REQUIRE(hs.total_slots(1) == 7);
    REQUIRE(hs.total_slots(2) == 7);
    REQUIRE(hs.total_slots(3) == 5);
    REQUIRE(hs.total_slots(4) == 4);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(21);
    REQUIRE(hs.total_slots(1) == 7);
    REQUIRE(hs.total_slots(2) == 7);
    REQUIRE(hs.total_slots(3) == 6);
    REQUIRE(hs.total_slots(4) == 4);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_wisdom(15);
    REQUIRE(hs.total_slots(1) == 6);
    REQUIRE(hs.total_slots(2) == 5);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    hs.set_caster_level(8);
    REQUIRE(hs.total_slots(1) == 5);
    REQUIRE(hs.total_slots(2) == 4);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 0);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);
}

TEST_CASE("[TC-HOLS.018] Casters cannot pray for unaccessible spells", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};

    REQUIRE_FALSE(hs.pray_for_spell(Defs::priest_spell::cure_critical_wounds));
}

TEST_CASE("[TC-HOLS.019] When caster's wisdom score decreases bonus slots in excess are forcibly freed", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_wisdom(21);
    hs.set_caster_level(9);

    REQUIRE(hs.total_slots(1) == 7);
    REQUIRE(hs.total_slots(2) == 7);
    REQUIRE(hs.total_slots(3) == 6);
    REQUIRE(hs.total_slots(4) == 4);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::create_water));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_light_wounds));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::bless));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::bless));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::bless));

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::animate_dead));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::glyph_of_warding));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::negative_plane_protection));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::prayer));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::remove_curse));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::remove_paralysis));

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_serious_wounds));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::hallucinatory_forest));
    REQUIRE(hs.pray_for_spell(Defs::priest_spell::neutralize_poison));

    REQUIRE(hs.pray_for_spell(Defs::priest_spell::cure_critical_wounds));
    
    REQUIRE(hs.used_slots(1) == 7);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.used_slots(3) == 6);
    REQUIRE(hs.used_slots(4) == 3);
    REQUIRE(hs.used_slots(5) == 1);
    REQUIRE(hs.used_slots(6) == 0);
    REQUIRE(hs.used_slots(7) == 0);

    hs.set_caster_wisdom(12);

    REQUIRE(hs.total_slots(1) == 4);
    REQUIRE(hs.total_slots(2) == 4);
    REQUIRE(hs.total_slots(3) == 3);
    REQUIRE(hs.total_slots(4) == 2);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.total_slots(6) == 0);
    REQUIRE(hs.total_slots(7) == 0);
    
    REQUIRE(hs.used_slots(1) == 4);
    REQUIRE(hs.used_slots(2) == 0);
    REQUIRE(hs.used_slots(3) == 3);
    REQUIRE(hs.used_slots(4) == 2);
    REQUIRE(hs.used_slots(5) == 1);
    REQUIRE(hs.used_slots(6) == 0);
    REQUIRE(hs.used_slots(7) == 0);
}

TEST_CASE("[TC-HOLS.020] When casters's level decreases unreachable spell levels are no longer accessible", "[spells, holy_symbol]" )
{
    HolySymbol hs{Defs::character_class::cleric};
    hs.set_caster_level(9);

    auto spellId = Defs::priest_spell::raise_dead;
    REQUIRE(hs.pray_for_spell(spellId));
    REQUIRE(hs[spellId] == 1);
    REQUIRE(hs.total_slots(5) == 1);
    REQUIRE(hs.free_slots(5) == 0);
    REQUIRE(hs.used_slots(5) == 1);

    hs.set_caster_level(8);
    REQUIRE_THROWS_AS(hs[spellId], SpellException<Defs::priest_spell>);
    REQUIRE(hs.total_slots(5) == 0);
    REQUIRE(hs.free_slots(5) == 0);
    REQUIRE(hs.used_slots(5) == 0);
}