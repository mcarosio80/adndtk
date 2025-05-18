//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <common_types.h>
#include <saving_throws.h>

using namespace Adndtk;

TEST_CASE("[TC-SAVT.001] Saving throws for warrior change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::warrior;
    ExperienceLevel lvl{};

    lvl = 0;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 20);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 18);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 19);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 18);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 18);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 16);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 17);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 16);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 15);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 15);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
}

TEST_CASE("[TC-SAVT.002] Saving throws for wizard change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::wizard;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);

    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 7);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 3);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 3);
}

TEST_CASE("[TC-SAVT.003] Saving throws for priest change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::priest;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 13);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 11);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 9);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 2);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
}

TEST_CASE("[TC-SAVT.004] Saving throws for rogue change according to experience level", "[saving_throws]" )
{
    Defs::character_class_type cType = Defs::character_class_type::rogue;
    ExperienceLevel lvl{};

    lvl = 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 16);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 14);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 15);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 12);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 14);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 10);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 13);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 10);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 8);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 12);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 9);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 7);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 6);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 6);
    
    lvl += 1;
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::breath_weapon) == 11);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::death_magic) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::paralysation) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::petrification) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::poison) == 8);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::polymorph) == 4);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::rod) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::spell) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::staff) == 5);
    REQUIRE(SavingThrows::get_instance().get_score(cType, lvl, Defs::saving_throw::wand) == 5);
}