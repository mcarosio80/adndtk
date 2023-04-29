//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-ADVT.001] Advancement table for fighter contains expected values", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
    REQUIRE(advTable.get_xp_for_level(cls, 2) == 2000);
    REQUIRE(advTable.get_xp_for_level(cls, 3) == 4000);
    REQUIRE(advTable.get_xp_for_level(cls, 4) == 8000);
    REQUIRE(advTable.get_xp_for_level(cls, 5) == 16000);
    REQUIRE(advTable.get_xp_for_level(cls, 6) == 32000);
    REQUIRE(advTable.get_xp_for_level(cls, 7) == 64000);
    REQUIRE(advTable.get_xp_for_level(cls, 8) == 125000);
    REQUIRE(advTable.get_xp_for_level(cls, 9) == 250000);
    REQUIRE(advTable.get_xp_for_level(cls, 10) == 500000);
    REQUIRE(advTable.get_xp_for_level(cls, 11) == 750000);
    REQUIRE(advTable.get_xp_for_level(cls, 12) == 1000000);
    REQUIRE(advTable.get_xp_for_level(cls, 13) == 1250000);
    REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
    REQUIRE(advTable.get_xp_for_level(cls, 15) == 1750000);
    REQUIRE(advTable.get_xp_for_level(cls, 16) == 2000000);
    REQUIRE(advTable.get_xp_for_level(cls, 17) == 2250000);
    REQUIRE(advTable.get_xp_for_level(cls, 18) == 2500000);
    REQUIRE(advTable.get_xp_for_level(cls, 19) == 2750000);
    REQUIRE(advTable.get_xp_for_level(cls, 20) == 3000000);
}

TEST_CASE("[TC-ADVT.002] Advancement table for ranger/paladin contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::ranger,
        Defs::character_class::paladin
    };
    auto advTable = Adndtk::Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 2250);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 4500);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 9000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 18000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 36000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 75000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 150000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 300000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 600000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 900000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 1200000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1500000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1800000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 2100000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 2400000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2700000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 3000000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 3300000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 3600000);
    }
}

TEST_CASE("[TC-ADVT.003] Advancement table for mage/specialist contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::mage,
        Defs::character_class::abjurer,
        Defs::character_class::conjurer,
        Defs::character_class::diviner,
        Defs::character_class::enchanter,
        Defs::character_class::illusionist,
        Defs::character_class::abjurer,
        Defs::character_class::invoker,
        Defs::character_class::necromancer,
        Defs::character_class::transmuter
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 2500);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 5000);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 10000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 20000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 40000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 60000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 90000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 135000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 250000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 375000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 750000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1125000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1875000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 2250000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2625000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 3000000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 3375000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 3750000);
    }
}

TEST_CASE("[TC-ADVT.004] Advancement table for cleric/preist_of_specific_mythos contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::cleric,
        Defs::character_class::preist_of_specific_mythos,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 1500);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 3000);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 6000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 13000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 27500);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 55000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 110000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 225000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 450000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 675000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 900000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 1125000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 1350000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1575000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 1800000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 2025000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 2250000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 2475000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 2700000);
    }
}

TEST_CASE("[TC-ADVT.005] Advancement table for druid contains expected values", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
    REQUIRE(advTable.get_xp_for_level(cls, 2) == 2000);
    REQUIRE(advTable.get_xp_for_level(cls, 3) == 4000);
    REQUIRE(advTable.get_xp_for_level(cls, 4) == 7500);
    REQUIRE(advTable.get_xp_for_level(cls, 5) == 12500);
    REQUIRE(advTable.get_xp_for_level(cls, 6) == 20000);
    REQUIRE(advTable.get_xp_for_level(cls, 7) == 35000);
    REQUIRE(advTable.get_xp_for_level(cls, 8) == 60000);
    REQUIRE(advTable.get_xp_for_level(cls, 9) == 90000);
    REQUIRE(advTable.get_xp_for_level(cls, 10) == 125000);
    REQUIRE(advTable.get_xp_for_level(cls, 11) == 200000);
    REQUIRE(advTable.get_xp_for_level(cls, 12) == 300000);
    REQUIRE(advTable.get_xp_for_level(cls, 13) == 750000);
    REQUIRE(advTable.get_xp_for_level(cls, 14) == 1500000);
    REQUIRE(advTable.get_xp_for_level(cls, 15) == 3000000);
    REQUIRE(advTable.get_xp_for_level(cls, 16) == 3500000);
    REQUIRE(advTable.get_xp_for_level(cls, 17) == 4000000);
    REQUIRE(advTable.get_xp_for_level(cls, 18) == 4500000);
    REQUIRE(advTable.get_xp_for_level(cls, 19) == 5000000);
    REQUIRE(advTable.get_xp_for_level(cls, 20) == 5500000);
}

TEST_CASE("[TC-ADVT.006] Advancement table for thief/bard contains expected values", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::thief,
        Defs::character_class::bard,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_xp_for_level(cls, 1) == 0);
        REQUIRE(advTable.get_xp_for_level(cls, 2) == 1250);
        REQUIRE(advTable.get_xp_for_level(cls, 3) == 2500);
        REQUIRE(advTable.get_xp_for_level(cls, 4) == 5000);
        REQUIRE(advTable.get_xp_for_level(cls, 5) == 10000);
        REQUIRE(advTable.get_xp_for_level(cls, 6) == 20000);
        REQUIRE(advTable.get_xp_for_level(cls, 7) == 40000);
        REQUIRE(advTable.get_xp_for_level(cls, 8) == 70000);
        REQUIRE(advTable.get_xp_for_level(cls, 9) == 110000);
        REQUIRE(advTable.get_xp_for_level(cls, 10) == 160000);
        REQUIRE(advTable.get_xp_for_level(cls, 11) == 220000);
        REQUIRE(advTable.get_xp_for_level(cls, 12) == 440000);
        REQUIRE(advTable.get_xp_for_level(cls, 13) == 660000);
        REQUIRE(advTable.get_xp_for_level(cls, 14) == 880000);
        REQUIRE(advTable.get_xp_for_level(cls, 15) == 1100000);
        REQUIRE(advTable.get_xp_for_level(cls, 16) == 1320000);
        REQUIRE(advTable.get_xp_for_level(cls, 17) == 1540000);
        REQUIRE(advTable.get_xp_for_level(cls, 18) == 1760000);
        REQUIRE(advTable.get_xp_for_level(cls, 19) == 1980000);
        REQUIRE(advTable.get_xp_for_level(cls, 20) == 2200000);
    }
}

TEST_CASE("[TC-ADVT.007] Level lookup for fighter yields expected value", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_level(cls, 0) == 1);
    REQUIRE(advTable.get_level(cls, 1900) == 1);
    REQUIRE(advTable.get_level(cls, 2000) == 2);
    REQUIRE(advTable.get_level(cls, 7999) == 3);
    REQUIRE(advTable.get_level(cls, 8000) == 4);
    REQUIRE(advTable.get_level(cls, 200000) == 8);
    REQUIRE(advTable.get_level(cls, 500001) == 10);
    REQUIRE(advTable.get_level(cls, 1000100) == 12);
    REQUIRE(advTable.get_level(cls, 2000010) == 16);
    REQUIRE(advTable.get_level(cls, 3000000) == 20);
    REQUIRE(advTable.get_level(cls, 3400000) == 21);
}

TEST_CASE("[TC-ADVT.008] Level lookup for ranger/paladin yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::ranger,
        Defs::character_class::paladin,
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1900) == 1);
        REQUIRE(advTable.get_level(cls, 2250) == 2);
        REQUIRE(advTable.get_level(cls, 8999) == 3);
        REQUIRE(advTable.get_level(cls, 9000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 8);
        REQUIRE(advTable.get_level(cls, 500001) == 9);
        REQUIRE(advTable.get_level(cls, 1000100) == 11);
        REQUIRE(advTable.get_level(cls, 2000010) == 14);
        REQUIRE(advTable.get_level(cls, 3000000) == 18);
        REQUIRE(advTable.get_level(cls, 3400000) == 19);
        REQUIRE(advTable.get_level(cls, 3600000) == 20);
        REQUIRE(advTable.get_level(cls, 3700000) == 20);
        REQUIRE(advTable.get_level(cls, 3900000) == 21);
    }
}

TEST_CASE("[TC-ADVT.009] Level lookup for mage/specialist yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::mage,
        Defs::character_class::abjurer,
        Defs::character_class::conjurer,
        Defs::character_class::diviner,
        Defs::character_class::enchanter,
        Defs::character_class::illusionist,
        Defs::character_class::invoker,
        Defs::character_class::necromancer,
        Defs::character_class::transmuter
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1900) == 1);
        REQUIRE(advTable.get_level(cls, 2500) == 2);
        REQUIRE(advTable.get_level(cls, 9999) == 3);
        REQUIRE(advTable.get_level(cls, 10000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 9);
        REQUIRE(advTable.get_level(cls, 500001) == 11);
        REQUIRE(advTable.get_level(cls, 1000100) == 12);
        REQUIRE(advTable.get_level(cls, 2000010) == 15);
        REQUIRE(advTable.get_level(cls, 3000000) == 18);
        REQUIRE(advTable.get_level(cls, 3400000) == 19);
        REQUIRE(advTable.get_level(cls, 3600000) == 19);
        REQUIRE(advTable.get_level(cls, 3750000) == 20);
        REQUIRE(advTable.get_level(cls, 4100000) == 20);
        REQUIRE(advTable.get_level(cls, 4200000) == 21);
    }
}

TEST_CASE("[TC-ADVT.010] Level lookup for cleric/preist_of_specific_mythos yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::cleric,
        Defs::character_class::preist_of_specific_mythos
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1400) == 1);
        REQUIRE(advTable.get_level(cls, 1500) == 2);
        REQUIRE(advTable.get_level(cls, 5999) == 3);
        REQUIRE(advTable.get_level(cls, 6000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 8);
        REQUIRE(advTable.get_level(cls, 500001) == 10);
        REQUIRE(advTable.get_level(cls, 1000100) == 12);
        REQUIRE(advTable.get_level(cls, 2000010) == 16);
        REQUIRE(advTable.get_level(cls, 2030000) == 17);
        REQUIRE(advTable.get_level(cls, 2500000) == 19);
        REQUIRE(advTable.get_level(cls, 2700000) == 20);
        REQUIRE(advTable.get_level(cls, 2924000) == 20);
        REQUIRE(advTable.get_level(cls, 2925000) == 21);
        REQUIRE(advTable.get_level(cls, 3000000) == 21);
    }
}

TEST_CASE("[TC-ADVT.010] Level lookup for druid yields expected value", "[advancement][experience]" )
{
    Defs::character_class cls = Defs::character_class::druid;
    auto advTable = Cyclopedia::get_instance().advancement_table();

    REQUIRE(advTable.get_level(cls, 0) == 1);
    REQUIRE(advTable.get_level(cls, 1999) == 1);
    REQUIRE(advTable.get_level(cls, 2000) == 2);
    REQUIRE(advTable.get_level(cls, 5999) == 3);
    REQUIRE(advTable.get_level(cls, 7500) == 4);
    REQUIRE(advTable.get_level(cls, 200000) == 11);
    REQUIRE(advTable.get_level(cls, 500001) == 12);
    REQUIRE(advTable.get_level(cls, 1000100) == 13);
    REQUIRE(advTable.get_level(cls, 2925000) == 14);
    REQUIRE(advTable.get_level(cls, 3000000) == 15);
    REQUIRE(advTable.get_level(cls, 3499999) == 15);
    REQUIRE(advTable.get_level(cls, 3500000) == 16);
    REQUIRE(advTable.get_level(cls, 3999999) == 16);
    REQUIRE(advTable.get_level(cls, 4000000) == 17);
    REQUIRE(advTable.get_level(cls, 4499999) == 17);
    REQUIRE(advTable.get_level(cls, 4500000) == 18);
    REQUIRE(advTable.get_level(cls, 4999999) == 18);
    REQUIRE(advTable.get_level(cls, 5000000) == 19);
    REQUIRE(advTable.get_level(cls, 5499999) == 19);
    REQUIRE(advTable.get_level(cls, 5500000) == 20);
    REQUIRE(advTable.get_level(cls, 5999999) == 20);
    REQUIRE(advTable.get_level(cls, 6000000) == 21);
}

TEST_CASE("[TC-ADVT.010] Level lookup for thief/bard yields expected value", "[advancement][experience]" )
{
    std::vector<Defs::character_class> classes{
        Defs::character_class::thief,
        Defs::character_class::bard
    };
    auto advTable = Cyclopedia::get_instance().advancement_table();

    for (auto& cls : classes)
    {
        REQUIRE(advTable.get_level(cls, 0) == 1);
        REQUIRE(advTable.get_level(cls, 1200) == 1);
        REQUIRE(advTable.get_level(cls, 1250) == 2);
        REQUIRE(advTable.get_level(cls, 4999) == 3);
        REQUIRE(advTable.get_level(cls, 5000) == 4);
        REQUIRE(advTable.get_level(cls, 200000) == 10);
        REQUIRE(advTable.get_level(cls, 500001) == 12);
        REQUIRE(advTable.get_level(cls, 1000100) == 14);
        REQUIRE(advTable.get_level(cls, 1500000) == 16);
        REQUIRE(advTable.get_level(cls, 1550000) == 17);
        REQUIRE(advTable.get_level(cls, 2000000) == 19);
        REQUIRE(advTable.get_level(cls, 2200000) == 20);
        REQUIRE(advTable.get_level(cls, 2400000) == 20);
        REQUIRE(advTable.get_level(cls, 2420000) == 21);
    }
}