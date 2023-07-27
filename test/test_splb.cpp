//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-SPLB.001] First level caster cannot learn higher level spells", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::fireball) == AddSpellResult::level_not_available);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::cone_of_cold) == AddSpellResult::level_not_available);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::melfs_acid_arrow) == AddSpellResult::level_not_available);
}

TEST_CASE("[TC-SPLB.002] Abjurers cannot learn spells of opposite schools (Alteration, Illusion)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::abjurer;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::affect_normal_fires) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::burning_hands) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::change_self) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::nystuls_magical_aura) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.003] Conjurers cannot learn spells of opposite schools (Greater Divination, Invocation)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::conjurer;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::alarm) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::tensers_floating_disc) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::wall_of_fog) == AddSpellResult::school_not_allowed);

    book.set_caster_level(9);
    book.set_caster_intelligence(10);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::contact_other_plane) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::false_vision) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.004] Diviners cannot learn spells of opposite schools (Conjuration, Summoning)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::diviner;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::mount) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::unseen_servant) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.005] Enchanters cannot learn spells of opposite schools (Invocation, Necromancy)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::enchanter;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::chill_touch) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.006] Illusionists cannot learn spells of opposite schools (Necromancy, Invocation, Abjuration)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::illusionist;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::chill_touch) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::alarm) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::protection_from_evil) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.007] Invokers cannot learn spells of opposite schools (Enchantment, Conjuration)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::invoker;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::armor) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::gaze_reflection) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::sleep) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::taunt) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.008] Necromancers cannot learn spells of opposite schools (Illusion, Enchantment)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::necromancer;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::audible_glamer) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::phantasmal_force) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::cantrip) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::sleep) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.009] Transmuters cannot learn spells of opposite schools (Abjuration, Necromancy)", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::transmuter;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;
    
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::chill_touch) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::alarm) == AddSpellResult::school_not_allowed);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::protection_from_evil) == AddSpellResult::school_not_allowed);
}

TEST_CASE("[TC-SPLB.010] Conjurers can access Lesser Divination school", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::conjurer;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::identify) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::read_magic) == AddSpellResult::success);
}

TEST_CASE("[TC-SPLB.011] Mages cannot exceed the maximum number of spells per level according to their intelligence score", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::enlarge) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::feather_fall) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::find_familiar) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::friends) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::gaze_reflection) == AddSpellResult::no_capacity);

    REQUIRE(book[Defs::wizard_spell::detect_magic] == 0);
    REQUIRE(book[Defs::wizard_spell::detect_undead] == 0);
    REQUIRE(book[Defs::wizard_spell::enlarge] == 0);
    REQUIRE(book[Defs::wizard_spell::feather_fall] == 0);
    REQUIRE(book[Defs::wizard_spell::find_familiar] == 0);
    REQUIRE(book[Defs::wizard_spell::friends] == 0);
    REQUIRE_THROWS_AS(book[Defs::wizard_spell::gaze_reflection], std::runtime_error);

    book.set_caster_intelligence(10);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::gaze_reflection) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::no_capacity);

    REQUIRE(book[Defs::wizard_spell::gaze_reflection] == 0);
    REQUIRE_THROWS_AS(book[Defs::wizard_spell::magic_missile], std::runtime_error);
}

TEST_CASE("[TC-SPLB.012] Spells cannot be scribed multiple times", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book[Defs::wizard_spell::detect_magic] == 0);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::existing);
    REQUIRE(book[Defs::wizard_spell::detect_magic] == 0);
}

TEST_CASE("[TC-SPLB.013] Spells deleted from the book, are no longer available", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    auto spellId = Defs::wizard_spell::detect_magic;
    REQUIRE(book.scribe_scroll(spellId) == AddSpellResult::success);
    REQUIRE(book[spellId] == 0);
    REQUIRE(book.delete_from_book(spellId));
    REQUIRE_THROWS_AS(book[spellId], std::runtime_error);
}

TEST_CASE("[TC-SPLB.014] Deleted spells can be written again", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    auto spellId = Defs::wizard_spell::detect_magic;
    REQUIRE(book.scribe_scroll(spellId) == AddSpellResult::success);
    REQUIRE(book[spellId] == 0);
    REQUIRE(book.delete_from_book(spellId));
    REQUIRE_THROWS_AS(book[spellId], std::runtime_error);
    REQUIRE(book.scribe_scroll(spellId) == AddSpellResult::success);
    REQUIRE(book[spellId] == 0);
}

TEST_CASE("[TC-SPLB.015] Casters can memorise as many spells as their level allows for", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};
    book.set_caster_level(5);

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::find_familiar) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::feather_fall) == AddSpellResult::success);

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::melfs_acid_arrow) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::strength) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::scare) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::summon_swarm) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::stinking_cloud) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::web) == AddSpellResult::success);

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::lightning_bolt) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::spectral_force) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::fireball) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::flame_arrow) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::leomunds_tiny_hut) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::item) == AddSpellResult::success);

    std::vector<short> freeSlots{4,2,1,0,0,0,0,0,0};
    std::vector<short> usedSlots{0,0,0,0,0,0,0,0,0};

    for (SpellLevel sl = 1; sl<=9; ++sl)
    {
        REQUIRE(book.free_slots(sl) == freeSlots[sl - 1]);
        REQUIRE(book.used_slots(sl) == usedSlots[sl - 1]);
    }

    REQUIRE(book.memorise(Defs::wizard_spell::detect_magic));
    REQUIRE(book.memorise(Defs::wizard_spell::magic_missile));
    freeSlots[0] -= 2;
    usedSlots[0] += 2;

    for (SpellLevel sl = 1; sl<=9; ++sl)
    {
        REQUIRE(book.free_slots(sl) == freeSlots[sl - 1]);
        REQUIRE(book.used_slots(sl) == usedSlots[sl - 1]);
    }

    REQUIRE(book.memorise(Defs::wizard_spell::melfs_acid_arrow));
    REQUIRE(book.memorise(Defs::wizard_spell::strength));
    REQUIRE_FALSE(book.memorise(Defs::wizard_spell::scare));
    freeSlots[1] -= 2;
    usedSlots[1] += 2;

    for (SpellLevel sl = 1; sl<=9; ++sl)
    {
        REQUIRE(book.free_slots(sl) == freeSlots[sl - 1]);
        REQUIRE(book.used_slots(sl) == usedSlots[sl - 1]);
    }

    REQUIRE(book.memorise(Defs::wizard_spell::lightning_bolt));
    REQUIRE_FALSE(book.memorise(Defs::wizard_spell::spectral_force));
    freeSlots[2] -= 1;
    usedSlots[2] += 1;
    
    for (SpellLevel sl = 1; sl<=9; ++sl)
    {
        REQUIRE(book.free_slots(sl) == freeSlots[sl - 1]);
        REQUIRE(book.used_slots(sl) == usedSlots[sl - 1]);
    }
}

TEST_CASE("[TC-SPLB.016] Casters can remove spells and free up slots", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};
    book.set_caster_level(5);

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::find_familiar) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::feather_fall) == AddSpellResult::success);

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::melfs_acid_arrow) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::strength) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::scare) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::summon_swarm) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::stinking_cloud) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::web) == AddSpellResult::success);

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::lightning_bolt) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::spectral_force) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::fireball) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::flame_arrow) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::leomunds_tiny_hut) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::item) == AddSpellResult::success);

    std::vector<short> freeSlots{4,2,1,0,0,0,0,0,0};
    std::vector<short> usedSlots{0,0,0,0,0,0,0,0,0};

    REQUIRE(book.memorise(Defs::wizard_spell::detect_magic));
    REQUIRE(book.memorise(Defs::wizard_spell::magic_missile));
    REQUIRE(book.free_slots(1) == 2);
    REQUIRE(book.used_slots(1) == 2);

    REQUIRE(book.memorise(Defs::wizard_spell::melfs_acid_arrow));
    REQUIRE(book.memorise(Defs::wizard_spell::strength));
    REQUIRE(book.free_slots(2) == 0);
    REQUIRE(book.used_slots(2) == 2);

    REQUIRE(book.remove(Defs::wizard_spell::melfs_acid_arrow));
    REQUIRE(book.free_slots(2) == 1);
    REQUIRE(book.used_slots(2) == 1);

    REQUIRE(book.remove(Defs::wizard_spell::magic_missile));
    REQUIRE(book.free_slots(1) == 3);
    REQUIRE(book.used_slots(1) == 1);
}

TEST_CASE("[TC-SPLB.017] Removed spells can no longer be memorised", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::success);

    REQUIRE(book.memorise(Defs::wizard_spell::detect_magic));
    REQUIRE(book.remove(Defs::wizard_spell::detect_magic));
    REQUIRE(book.delete_from_book(Defs::wizard_spell::detect_magic));
    REQUIRE_FALSE(book.memorise(Defs::wizard_spell::detect_magic));
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.memorise(Defs::wizard_spell::detect_magic));
}

TEST_CASE("[TC-SPLB.018] A deleted spell is also removed", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};
    book.set_caster_level(2);

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_undead) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::shield) == AddSpellResult::success);

    auto spellId = Defs::wizard_spell::magic_missile;

    REQUIRE(book[spellId] == 0);
    REQUIRE(book.free_slots(1) == 2);
    REQUIRE(book.used_slots(1) == 0);

    REQUIRE(book.memorise(spellId));
    REQUIRE(book[spellId] == 1);
    REQUIRE(book.free_slots(1) == 1);
    REQUIRE(book.used_slots(1) == 1);

    REQUIRE(book.delete_from_book(spellId));
    REQUIRE_THROWS_AS(book[spellId], std::runtime_error);
    REQUIRE(book.free_slots(1) == 2);
    REQUIRE(book.used_slots(1) == 0);
}

TEST_CASE("[TC-SPLB.019] Deleted spells can no longer be memorised", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::mage;
    auto raceId = Defs::race::human;
    SpellBook book{cls, raceId};

    OptionalRules::get_instance().option<bool>(Option::scribe_scroll_always_succeeds) = true;

    REQUIRE(book.scribe_scroll(Defs::wizard_spell::detect_magic) == AddSpellResult::success);
    REQUIRE(book.scribe_scroll(Defs::wizard_spell::magic_missile) == AddSpellResult::success);

    auto spellId = Defs::wizard_spell::magic_missile;

    REQUIRE(book.delete_from_book(spellId));
    REQUIRE_FALSE(book.memorise(spellId));
}

TEST_CASE("[TC-SPLB.020] Gnomes can be Illusionists", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::illusionist;
    auto raceId = Defs::race::gnome;
    REQUIRE_NOTHROW(SpellBook(cls, raceId));
}

TEST_CASE("[TC-SPLB.021] Half-elves cannot be Necromancers", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::necromancer;
    auto raceId = Defs::race::half_elf;
    REQUIRE_THROWS_AS(SpellBook(cls, raceId), std::runtime_error);
}

TEST_CASE("[TC-SPLB.022] Elves cannot be Invokers", "[spells, spell_book]" )
{
    Defs::character_class cls = Defs::character_class::invoker;
    auto raceId = Defs::race::elf;
    REQUIRE_THROWS_AS(SpellBook(cls, raceId), std::runtime_error);
}