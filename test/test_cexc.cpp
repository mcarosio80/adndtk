//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-CEXC.001] CoinExchange gives the correct conversion ratio from CP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::copper_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 1.0);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 0.1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 0.02);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.01);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.002);
}

TEST_CASE("[TC-CEXC.002] CoinExchange gives the correct conversion ratio from SP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::silver_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 0.2);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.02);
}

TEST_CASE("[TC-CEXC.003] CoinExchange gives the correct conversion ratio from EP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::electrum_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 50);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 0.5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.1);
}

TEST_CASE("[TC-CEXC.004] CoinExchange gives the correct conversion ratio from GP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::gold_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 100);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 2);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 1);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 0.2);
}

TEST_CASE("[TC-CEXC.005] CoinExchange gives the correct conversion ratio from SP to other coins", "[coin][exchange]" )
{
    Defs::coin c{Defs::coin::platinum_piece};
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::copper_piece) == 500);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::silver_piece) == 50);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::electrum_piece) == 10);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::gold_piece) == 5);
    REQUIRE(CoinExchange::get_instance().get_conversion_ratio(c, Defs::coin::platinum_piece) == 1);
}