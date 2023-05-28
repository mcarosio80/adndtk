//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-MONB.001] Availability of money in the same coin is granted for sum less then or equal to the value stored", "[money_bag]" )
{
    const uint32_t value = 100;
    std::vector<Defs::coin> coins = {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece};
    
    for (auto& c : coins)
    {
        MoneyBag mb;
        mb.add(c, value);

        REQUIRE(mb.check_availability(c, value-10));
        REQUIRE(mb.check_availability(c, value));
        REQUIRE_FALSE(mb.check_availability(c, value+1));
    }
}

TEST_CASE("[TC-MONB.002] Availability of money in different coin is granted for sum less then or equal to the value of PP stored", "[money_bag]" )
{
    const uint32_t value = 10000;
    std::vector<Defs::coin> coins = {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece};
    
    for (auto& c1 : coins)
    {
        MoneyBag mb;
        mb.add(c1, value);

        for (auto& c2 : coins)
        {
            auto fx = CoinExchange::get_instance().get_conversion_ratio(c1, c2);
            auto amount = value * fx;
            REQUIRE(mb.check_availability(c2, amount - 10));
            REQUIRE(mb.check_availability(c2, amount));
            REQUIRE_FALSE(mb.check_availability(c2, amount + 10));
        }
    }
}

TEST_CASE("[TC-MONB.003] Check of availability of exact amounts shows expected results", "[money_bag]" )
{
    const uint32_t value = 100;
    std::vector<Defs::coin> coins = {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece};
    
    for (auto& c1 : coins)
    {
        MoneyBag mb;
        mb.add(c1, value);

        for (auto& c2 : coins)
        {
            REQUIRE(mb[c2] == ((c1 == c2) ? value : 0));
        }
    }
}

TEST_CASE("[TC-MONB.004] Adding coin objects reflects the same results as adding explicit amounts", "[money_bag]" )
{
    const uint32_t value = 10000;
    std::vector<Defs::coin> coins = {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece};
    
    for (auto& c1 : coins)
    {
        MoneyBag mb;
        Coin cn{c1, value};
        mb += cn;

        for (auto& c2 : coins)
        {
            auto fx = CoinExchange::get_instance().get_conversion_ratio(c1, c2);
            auto amount = value * fx;
            REQUIRE(mb.check_availability(c2, amount - 10));
            REQUIRE(mb.check_availability(c2, amount));
            REQUIRE_FALSE(mb.check_availability(c2, amount + 10));
        }
    }
}

TEST_CASE("[TC-MONB.005] Subtracting of money in the same coin is granted for sum less then or equal to the value stored", "[money_bag]" )
{
    constexpr uint32_t value = 100;
    constexpr uint32_t diff = 50;

    for (auto& c : {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece})
    {
        constexpr uint32_t remaining = value - diff;

        MoneyBag mb;
        mb.add(c, value);
        mb.subtract(c, diff);
        REQUIRE(mb.check_availability(c, remaining-10));
        REQUIRE(mb.check_availability(c, remaining));
        REQUIRE_FALSE(mb.check_availability(c, remaining+10));
    }
}

TEST_CASE("[TC-MONB.006] Subtracting more than what available throws exceptions", "[money_bag]" )
{
    constexpr uint32_t value = 100;

    for (auto& c : {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece})
    {
        MoneyBag mb;
        mb.add(c, value);
        REQUIRE_THROWS(mb.subtract(c, value+1));
    }
}

TEST_CASE("[TC-MONB.007] Normalisation of money bag returns expected results", "[money_bag]" )
{
    {
        MoneyBag mb;
        mb.add(Defs::coin::copper_piece, 100);
        mb.normalise();

        REQUIRE(mb[Defs::coin::platinum_piece] == 0);
        REQUIRE(mb[Defs::coin::gold_piece] == 1);
        REQUIRE(mb[Defs::coin::electrum_piece] == 0);
        REQUIRE(mb[Defs::coin::silver_piece] == 0);
        REQUIRE(mb[Defs::coin::copper_piece] == 0);
    }
    {
        MoneyBag mb;
        mb.add(Defs::coin::copper_piece, 100);
        mb.add(Defs::coin::silver_piece, 10);
        mb.add(Defs::coin::electrum_piece, 2);
        mb.add(Defs::coin::gold_piece, 1);
        mb.normalise();

        REQUIRE(mb[Defs::coin::platinum_piece] == 0);
        REQUIRE(mb[Defs::coin::gold_piece] == 4);
        REQUIRE(mb[Defs::coin::electrum_piece] == 0);
        REQUIRE(mb[Defs::coin::silver_piece] == 0);
        REQUIRE(mb[Defs::coin::copper_piece] == 0);
    }
    {
        MoneyBag mb;
        mb.add(Defs::coin::silver_piece, 10);
        mb.add(Defs::coin::electrum_piece, 10);
        mb.add(Defs::coin::gold_piece, 10);
        mb.normalise();

        REQUIRE(mb[Defs::coin::platinum_piece] == 3);
        REQUIRE(mb[Defs::coin::gold_piece] == 1);
        REQUIRE(mb[Defs::coin::electrum_piece] == 0);
        REQUIRE(mb[Defs::coin::silver_piece] == 0);
        REQUIRE(mb[Defs::coin::copper_piece] == 0);
    }
}