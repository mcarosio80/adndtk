//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <coin.h>

using namespace Adndtk;

TEST_CASE("[TC-COIN.001] Comparison between coins of the same type yield expected results", "[coin]" )
{
    for (auto& t : {Defs::coin::copper_piece, Defs::coin::silver_piece, Defs::coin::electrum_piece, Defs::coin::gold_piece, Defs::coin::platinum_piece})
    {
        Coin c1{t, 10};
        Coin c2{t, 12};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
}

TEST_CASE("[TC-COIN.002] Comparison between coins of different same type yield expected results", "[coin]" )
{
    {
        Coin c1{Defs::coin::copper_piece, 10};
        Coin c2{Defs::coin::gold_piece, 10};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 100};
        Coin c2{Defs::coin::gold_piece, 10};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE_FALSE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 102};
        Coin c2{Defs::coin::gold_piece, 1};

        REQUIRE_FALSE(c1 == c2);
        REQUIRE(c1 != c2);
        REQUIRE_FALSE(c1 < c2);
        REQUIRE_FALSE(c1 <= c2);
        REQUIRE(c1 >= c2);
        REQUIRE(c1 > c2);
    }
    {
        Coin c1{Defs::coin::copper_piece, 100};
        Coin c2{Defs::coin::gold_piece, 1};

        REQUIRE(c1 == c2);
        REQUIRE_FALSE(c1 != c2);
        REQUIRE_FALSE(c1 < c2);
        REQUIRE(c1 <= c2);
        REQUIRE(c1 >= c2);
        REQUIRE_FALSE(c1 > c2);
    }
}

TEST_CASE("[TC-COIN.003] Comparison between coin and scalar yield expected results", "[coin]" )
{
    Coin c1{Defs::coin::gold_piece, 10};

    REQUIRE(c1 == 10);
    REQUIRE_FALSE(c1 == 11);
    REQUIRE(c1 != 11);
    REQUIRE_FALSE(c1 != 10);
    REQUIRE(c1 < 11);
    REQUIRE_FALSE(c1 < 10);
    REQUIRE_FALSE(c1 < 9);
    REQUIRE(c1 <= 11);
    REQUIRE(c1 <= 10);
    REQUIRE_FALSE(c1 <= 9);
    REQUIRE(c1 >= 9);
    REQUIRE(c1 >= 10);
    REQUIRE_FALSE(c1 >= 11);
    REQUIRE(c1 > 9);
    REQUIRE_FALSE(c1 > 10);
    REQUIRE_FALSE(c1 > 11);
}