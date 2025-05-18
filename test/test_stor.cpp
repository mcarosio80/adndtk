//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>
#include <optional>

#include <defs.h>
#include <store.h>
#include <options.h>
#include <query_result.h>
#include <cyclopedia.h>

using namespace Adndtk;

const char* frArmStore = "Friendly Arm's store";

TEST_CASE("[TC-STOR.001] Stores expose the name they are create with", "[store]" )
{
    REQUIRE(Store(frArmStore).store_name() == frArmStore);
}

TEST_CASE("[TC-STOR.002] Unsupplied items are not available", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::chain_mail;
    REQUIRE_FALSE(s.check_supply(equipId, 1));
}

TEST_CASE("[TC-STOR.003] Supplied items are available in specified number", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::chain_mail;
    s.supply(equipId, 2);
    REQUIRE(s.check_supply(equipId, 1));
    REQUIRE(s.check_supply(equipId, 2));
    REQUIRE_FALSE(s.check_supply(equipId, 3));
}

TEST_CASE("[TC-STOR.004] All items can be made available at the store (in specified number) ", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::chain_mail;
    s.supply_all(2);
    REQUIRE(s.check_supply(equipId, 1));
    REQUIRE(s.check_supply(equipId, 2));
    REQUIRE_FALSE(s.check_supply(equipId, 3));

    REQUIRE(s.check_supply(Defs::equipment::two_handed_sword, 1));
    REQUIRE(s.check_supply(Defs::equipment::warhammer, 1));
    REQUIRE(s.check_supply(Defs::equipment::barrel_small, 1));
    REQUIRE(s.check_supply(Defs::equipment::drakkar, 1));
    REQUIRE(s.check_supply(Defs::equipment::drakkar, 2));
    REQUIRE_FALSE(s.check_supply(Defs::equipment::drakkar, 3));
}

TEST_CASE("[TC-STOR.005] Every item will always be available if unlimited store supply option is enabled", "[store]" )
{
    Store s{frArmStore};
    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = true;

    REQUIRE(s.check_supply(Defs::equipment::drakkar, 8));
    REQUIRE(s.check_supply(Defs::equipment::medium_shield, 3));
    REQUIRE(s.check_supply(Defs::equipment::horsemans_flail, 2));
    REQUIRE(s.check_supply(Defs::equipment::battle_axe, 1));
    REQUIRE(s.check_supply(Defs::equipment::yoke_horse, 2));
    REQUIRE(s.check_supply(Defs::equipment::pigeon_homing, 4));
    REQUIRE(s.check_supply(Defs::equipment::pony, 14));
    REQUIRE(s.check_supply(Defs::equipment::chariot_war, 1));
    REQUIRE(s.check_supply(Defs::equipment::long_bow, 3));

    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = false;
}

TEST_CASE("[TC-STOR.006] Removed item are no longer in stock", "[store]" )
{
    Store s{frArmStore};
    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = false;

    auto equipId = Defs::equipment::long_sword;
    s.supply(equipId, 2);
    REQUIRE(s.check_supply(equipId, 2));
    s.remove(equipId, 1);
    REQUIRE_FALSE(s.check_supply(equipId, 2));
    REQUIRE(s.check_supply(equipId, 1));
}

TEST_CASE("[TC-STOR.007] Removing items has no effect if unlimited store supply option is enabled", "[store]" )
{
    Store s{frArmStore};
    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = true;

    auto equipId = Defs::equipment::long_sword;
    REQUIRE(s.check_supply(equipId, 5));
    REQUIRE(s.check_supply(equipId, 2));
    REQUIRE(s.check_supply(equipId, 1));
    s.supply(equipId, 2);
    REQUIRE(s.check_supply(equipId, 5));
    REQUIRE(s.check_supply(equipId, 2));
    REQUIRE(s.check_supply(equipId, 1));
    s.remove(equipId, 1);
    REQUIRE(s.check_supply(equipId, 5));
    REQUIRE(s.check_supply(equipId, 2));
    REQUIRE(s.check_supply(equipId, 1));

    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = false;
}

TEST_CASE("[TC-STOR.008] Counting items always returns the required quantity if unlimited store supply option is enabled", "[store]" )
{
    Store s{frArmStore};
    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = true;

    auto equipId = Defs::equipment::long_sword;
    REQUIRE(s.check_supply(equipId, 1));
    REQUIRE(s.check_supply(equipId, 10));
    REQUIRE(s.check_supply(equipId, 100));

    OptionalRules::get_instance().option<bool>(Option::unlimited_store_supply) = false;
}

TEST_CASE("[TC-STOR.009] Stores have a starting money availability between 1000 and 10000 CP", "[store]" )
{
    Store s{frArmStore};

    auto coinId = Defs::coin::copper_piece;
    const uint32_t minAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_min_cp_starting_amount)};
    const uint32_t maxAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_max_cp_starting_amount)};
    REQUIRE(s.check_availability(coinId, minAmt-1, true));
    REQUIRE_FALSE(s.check_availability(coinId, maxAmt+1, true));

    REQUIRE(s[coinId] >= minAmt);
    REQUIRE(s[coinId] <= maxAmt);
}

TEST_CASE("[TC-STOR.010] Stores have a starting money availability between 1000 and 5000 SP", "[store]" )
{
    Store s{frArmStore};

    auto coinId = Defs::coin::silver_piece;
    const uint32_t minAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_min_sp_starting_amount)};
    const uint32_t maxAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_max_sp_starting_amount)};
    REQUIRE(s.check_availability(coinId, minAmt-1, true));
    REQUIRE_FALSE(s.check_availability(coinId, maxAmt+1, true));

    REQUIRE(s[coinId] >= minAmt);
    REQUIRE(s[coinId] <= maxAmt);
}

TEST_CASE("[TC-STOR.011] Stores have a starting money availability between 1000 and 2000 EP", "[store]" )
{
    Store s{frArmStore};

    auto coinId = Defs::coin::electrum_piece;
    const uint32_t minAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_min_ep_starting_amount)};
    const uint32_t maxAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_max_ep_starting_amount)};
    REQUIRE(s.check_availability(coinId, minAmt-1, true));
    REQUIRE_FALSE(s.check_availability(coinId, maxAmt+1, true));

    REQUIRE(s[coinId] >= minAmt);
    REQUIRE(s[coinId] <= maxAmt);
}

TEST_CASE("[TC-STOR.012] Stores have a starting money availability between 100 and 500 GP", "[store]" )
{
    Store s{frArmStore};

    auto coinId = Defs::coin::gold_piece;
    const uint32_t minAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_min_gp_starting_amount)};
    const uint32_t maxAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_max_gp_starting_amount)};
    REQUIRE(s.check_availability(coinId, minAmt-1, true));
    REQUIRE_FALSE(s.check_availability(coinId, maxAmt+1, true));

    REQUIRE(s[coinId] >= minAmt);
    REQUIRE(s[coinId] <= maxAmt);
}

TEST_CASE("[TC-STOR.013] Stores have a starting money availability between 100 and 200 PP", "[store]" )
{
    Store s{frArmStore};

    auto coinId = Defs::coin::platinum_piece;
    const uint32_t minAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_min_pp_starting_amount)};
    const uint32_t maxAmt{OptionalRules::get_instance().option<uint32_t>(Option::store_max_pp_starting_amount)};
    REQUIRE(s.check_availability(coinId, minAmt-1, true));
    REQUIRE_FALSE(s.check_availability(coinId, maxAmt+1, true));

    REQUIRE(s[coinId] >= minAmt);
    REQUIRE(s[coinId] <= maxAmt);
}

TEST_CASE("[TC-STOR.014] Stores apply a price spread when price is not defined by an interval", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::long_sword;
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_equipment, equipId);
    const QueryResult& equipInfo = rs[0];

    auto currency = equipInfo.as<Defs::coin>("cost_coin");
    auto price = equipInfo.as<double>("cost_min");
    REQUIRE(s.get_buy_price(equipId) <= price);
    REQUIRE(s.get_sell_price(equipId) >= price);
}

TEST_CASE("[TC-STOR.015] Stores apply a random price in price interval, never less than minimum, never more than maximum", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::full_plate;
    auto rs = Cyclopedia::get_instance().exec_prepared_statement<Defs::equipment>(Query::select_equipment, equipId);
    const QueryResult& equipInfo = rs[0];

    auto currency = equipInfo.as<Defs::coin>("cost_coin");
    auto costMin = equipInfo.as<double>("cost_min");
    auto costMax = equipInfo.as<double>("cost_max");

    auto buyPrice = s.get_buy_price(equipId);
    auto sellPrice = s.get_sell_price(equipId);
    REQUIRE(buyPrice >= costMin);
    REQUIRE(buyPrice < sellPrice);
    REQUIRE(sellPrice <= costMax);
}

TEST_CASE("[TC-STOR.016] Once a variable price is set, the store keeps applying the same price", "[store]" )
{
    Store s{frArmStore};

    auto equipId = Defs::equipment::full_plate;

    auto buyPrice1 = s.get_buy_price(equipId);
    auto buyPrice2 = s.get_buy_price(equipId);
    auto buyPrice3 = s.get_buy_price(equipId);
    REQUIRE(buyPrice1 == buyPrice2);
    REQUIRE(buyPrice1 == buyPrice3);
}