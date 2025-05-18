//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include <optional>

#include <defs.h>
#include <inventory.h>
#include <common_types.h>

using namespace Adndtk;

TEST_CASE("[TC-INVE.001] Items can be added to the inventory", "[inventory]" )
{
    Defs::equipment itemId = Defs::equipment::khopesh;
    Inventory inv;
    REQUIRE(inv.add(Defs::equipment::backpack));
    REQUIRE(inv.add(itemId, 3));
    REQUIRE(inv.count_items(itemId) == 3);
}

TEST_CASE("[TC-INVE.002] Items not in the inventory have count = 0", "[inventory]" )
{
    Inventory inv;
    REQUIRE(inv.count_items(Defs::equipment::bastard_sword) == 0);
}

TEST_CASE("[TC-INVE.003] Items can be tested for presence", "[inventory]" )
{
    Inventory inv;
    auto battleAxeId = Defs::equipment::battle_axe;
    auto bastardSwordId = Defs::equipment::bastard_sword;
    auto dryRationId = Defs::equipment::dry_ration_per_week;
    
    REQUIRE(inv.add(Defs::equipment::backpack));
    REQUIRE(inv.add(battleAxeId));
    REQUIRE(inv.add(bastardSwordId));

    REQUIRE(inv.has_item(battleAxeId));
    REQUIRE(inv.has_item(bastardSwordId));
    REQUIRE_FALSE(inv.has_item(dryRationId));

    REQUIRE_FALSE(inv.has_item(Defs::equipment::halberd));
    REQUIRE_FALSE(inv.has_item(Defs::equipment::fishhook));
    REQUIRE_FALSE(inv.has_item(Defs::equipment::boots_soft));
}

TEST_CASE("[TC-INVE.004] Arrows/quarrels require a quiver", "[inventory]" )
{
    Inventory inv;
    auto sheafArrowId = Defs::equipment::sheaf_arrow;
    auto heavyQuarrelId = Defs::equipment::heavy_quarrel;
    auto quiverId = Defs::equipment::quiver;

    REQUIRE_THROWS_AS(inv.add(sheafArrowId, 20), std::runtime_error);
    REQUIRE_THROWS_AS(inv.add(heavyQuarrelId, 20), std::runtime_error);
    
    REQUIRE(inv.add(Defs::equipment::backpack));
    REQUIRE(inv.add(quiverId));
    REQUIRE(inv.add(sheafArrowId, 20));
    REQUIRE(inv.add(heavyQuarrelId, 20));
}

TEST_CASE("[TC-INVE.005] Items that cannot be carried on hands, like food rations, require a backpack", "[inventory]" )
{
    Inventory inv;
    auto dryRationId = Defs::equipment::dry_ration_per_week;
    auto packpackId = Defs::equipment::backpack;
    
    REQUIRE_THROWS_AS(inv.add(dryRationId), std::runtime_error);
    REQUIRE_FALSE(inv.has_item(dryRationId));

    REQUIRE(inv.add(packpackId));
    REQUIRE(inv.add(dryRationId));
    REQUIRE(inv.has_item(dryRationId));
}

TEST_CASE("[TC-INVE.006] Adding items increase inventory total weight", "[inventory]" )
{
    Inventory inv;

    REQUIRE(inv.total_weight() == Const::clothes_weight);

    auto twoHandedSwordId = Defs::equipment::two_handed_sword;
    REQUIRE(inv.add(Defs::equipment::backpack));
    REQUIRE(inv.add(twoHandedSwordId));
    REQUIRE(inv.total_weight() > Const::clothes_weight);
}

TEST_CASE("[TC-INVE.007] Removing items decrease inventory total weight", "[inventory]" )
{
    Inventory inv;

    REQUIRE(inv.total_weight() == Const::clothes_weight);

    auto twoHandedSwordId = Defs::equipment::two_handed_sword;
    REQUIRE(inv.add(Defs::equipment::backpack));
    auto wgt = inv.total_weight();
    REQUIRE(inv.add(twoHandedSwordId));
    REQUIRE(inv.total_weight() > wgt);

    REQUIRE(inv.remove(twoHandedSwordId));
    REQUIRE(inv.total_weight() == wgt);
}

TEST_CASE("[TC-INVE.008] Removing items not owned throws exceptions", "[inventory]" )
{
    Inventory inv;

    auto twoHandedSwordId = Defs::equipment::two_handed_sword;

    REQUIRE_FALSE(inv.remove(twoHandedSwordId));
    REQUIRE(inv.total_weight() == Const::clothes_weight);
    REQUIRE(inv.count_items(twoHandedSwordId) == 0);
    REQUIRE_FALSE(inv.has_item(twoHandedSwordId));
}

TEST_CASE("[TC-INVE.009] Items with no body slot assigned go into the backpack, if available", "[inventory]" )
{
    Inventory inv;
    auto backpackId = Defs::equipment::backpack;
    auto twoHandedSwordId = Defs::equipment::two_handed_sword;

    REQUIRE(inv.add(backpackId));
    REQUIRE(inv.find(backpackId) == Defs::body_slot::back);
    REQUIRE(inv.add(twoHandedSwordId));
    REQUIRE(inv.find(twoHandedSwordId) == Defs::body_slot::backpack);
}

TEST_CASE("[TC-INVE.010] Items with no body slot assigned go nowhere if backpack is not available", "[inventory]" )
{
    Inventory inv;
    auto twoHandedSwordId = Defs::equipment::two_handed_sword;

    REQUIRE_THROWS_AS(inv.add(twoHandedSwordId), std::runtime_error);
    REQUIRE_FALSE(inv.find(twoHandedSwordId).has_value());
}

TEST_CASE("[TC-INVE.011] Arrows/quarrels go into quiver if available", "[inventory]" )
{
    Inventory inv;
    auto quiverId = Defs::equipment::quiver;
    auto sheafArrowId = Defs::equipment::sheaf_arrow;
    auto heavyQuarrelID = Defs::equipment::heavy_quarrel;

    REQUIRE(inv.add(quiverId));
    REQUIRE(inv.add(sheafArrowId));
    REQUIRE(inv.find(sheafArrowId).has_value());

    REQUIRE(inv.add(heavyQuarrelID));
    REQUIRE(inv.find(heavyQuarrelID).has_value());
}

TEST_CASE("[TC-INVE.012] Arrows/quarrels go nowhere if quiver/backpack are not available", "[inventory]" )
{
    Inventory inv;
    auto sheafArrowId = Defs::equipment::sheaf_arrow;
    auto heavyQuarrelID = Defs::equipment::heavy_quarrel;

    REQUIRE_THROWS_AS(inv.add(sheafArrowId), std::runtime_error);
    REQUIRE_FALSE(inv.find(sheafArrowId).has_value());

    REQUIRE_THROWS_AS(inv.add(heavyQuarrelID), std::runtime_error);
    REQUIRE_FALSE(inv.find(heavyQuarrelID).has_value());
}

TEST_CASE("[TC-INVE.013] Items can be moved from one slot to another", "[inventory]" )
{
    Inventory inv;
    auto backpackId = Defs::equipment::backpack;
    auto longSwordId = Defs::equipment::long_sword;

    REQUIRE(inv.add(backpackId));
    REQUIRE(inv.add(longSwordId));
    REQUIRE(inv.find(longSwordId) == Defs::body_slot::backpack);
    REQUIRE(inv.move(longSwordId, Defs::body_slot::hands));
    REQUIRE(inv.find(longSwordId) == Defs::body_slot::hands);
}

TEST_CASE("[TC-INVE.014] Arrows/quarrels go to backpack first, then to quiver if available", "[inventory]" )
{
    Inventory inv;
    auto backpackId = Defs::equipment::backpack;
    auto quiverId = Defs::equipment::quiver;
    auto sheafArrowId = Defs::equipment::sheaf_arrow;
    auto heavyQuarrelID = Defs::equipment::heavy_quarrel;

    REQUIRE(inv.add(backpackId));
    REQUIRE(inv.add(sheafArrowId));
    REQUIRE(inv.add(heavyQuarrelID));
    REQUIRE(inv.find(sheafArrowId) == Defs::body_slot::backpack);
    REQUIRE(inv.find(heavyQuarrelID) == Defs::body_slot::backpack);

    REQUIRE_FALSE(inv.move(sheafArrowId, Defs::body_slot::quiver));
    REQUIRE_FALSE(inv.move(heavyQuarrelID, Defs::body_slot::quiver));

    REQUIRE(inv.add(quiverId));

    REQUIRE(inv.move(sheafArrowId, Defs::body_slot::quiver));
    REQUIRE(inv.move(heavyQuarrelID, Defs::body_slot::quiver));
}

TEST_CASE("[TC-INVE.015] Moving items does not affect total weight", "[inventory]" )
{
    Inventory inv;
    auto backpackId = Defs::equipment::backpack;
    auto longSwordId = Defs::equipment::long_sword;

    REQUIRE(inv.add(backpackId));
    REQUIRE(inv.add(longSwordId));
    auto wgt = inv.total_weight();
    REQUIRE(inv.move(longSwordId, Defs::body_slot::hands));
    REQUIRE(inv.total_weight() == wgt);
}