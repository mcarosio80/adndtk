//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <vector>

#include <market.h>

using namespace Adndtk;

TEST_CASE("[TC-MRKT.001] Market space is empty at the beginning", "[market]" )
{
    REQUIRE(Market::get_instance().count() == 0);
}

TEST_CASE("[TC-MRKT.002] Retrieving a stores which was not previously created raises an error", "[market]" )
{
    const char* friendlyArmStore = "Friendly Arm's store";
    const char* winthropStore = "Winthrop's store";
    
    REQUIRE(Market::get_instance().count() == 0);
    REQUIRE_THROWS_AS(Market::get_instance()[friendlyArmStore], std::runtime_error);
    REQUIRE_THROWS_AS(Market::get_instance()[winthropStore], std::runtime_error);

    REQUIRE(Market::get_instance().add(winthropStore).store_name() == winthropStore);
    REQUIRE(Market::get_instance().count() == 1);
    REQUIRE_THROWS_AS(Market::get_instance()[friendlyArmStore], std::runtime_error);

    Market::get_instance().clear();
    REQUIRE(Market::get_instance().count() == 0);
}