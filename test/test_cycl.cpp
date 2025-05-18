//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <cyclopedia.h>

using namespace Adndtk;

TEST_CASE("[TC-CYCL.001] Reading text data from knowledge base returns the whole text", "[cyclopedia]" )
{
    auto cls = Defs::character_class::fighter_mage;
    auto results = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(Query::select_character_class, cls);
    auto& r = results[0];

    REQUIRE(r.as<std::string>("long_name") == "Fighter Mage");
}

TEST_CASE("[TC-CYCL.002] Reading binaries data from knowledge base can be cast to enum class values", "[cyclopedia]" )
{
    auto clsId = Defs::character_class::fighter_mage;
    auto results = Cyclopedia::get_instance().exec_prepared_statement<Defs::character_class>(Query::select_character_class, clsId);
    auto& r = results[0];

    REQUIRE(r.as<Defs::character_class>("id") == clsId);
}