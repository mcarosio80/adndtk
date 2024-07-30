//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-TREA.001] Gems with no range value are always valued the same", "[treasure]" )
{
    auto coinType = Defs::coin::gold_piece;
    Coin gp10{Defs::coin::gold_piece, 10};
    Coin gp50{Defs::coin::gold_piece, 50};
    Coin gp100{Defs::coin::gold_piece, 100};
    Coin gp500{Defs::coin::gold_piece, 500};
    Coin gp1000{Defs::coin::gold_piece, 1000};
    Coin gp5000{Defs::coin::gold_piece, 5000};

    REQUIRE(Gem{Defs::gem::azurite}.value() == gp10);
    REQUIRE(Gem{Defs::gem::banded_agate}.value() == gp10);
    REQUIRE(Gem{Defs::gem::blue_quartz}.value() == gp10);
    REQUIRE(Gem{Defs::gem::eye_agate}.value() == gp10);
    REQUIRE(Gem{Defs::gem::hematite}.value() == gp10);
    REQUIRE(Gem{Defs::gem::lapis_lazuli}.value() == gp10);
    REQUIRE(Gem{Defs::gem::malachite}.value() == gp10);
    REQUIRE(Gem{Defs::gem::moss_agate}.value() == gp10);
    REQUIRE(Gem{Defs::gem::obsidian}.value() == gp10);
    REQUIRE(Gem{Defs::gem::rhodochrosite}.value() == gp10);
    REQUIRE(Gem{Defs::gem::tiger_eye_agate}.value() == gp10);
    REQUIRE(Gem{Defs::gem::turquoise}.value() == gp10);
    REQUIRE(Gem{Defs::gem::bloodstone}.value() == gp50);
    REQUIRE(Gem{Defs::gem::camelian}.value() == gp50);
    REQUIRE(Gem{Defs::gem::chalcedony}.value() == gp50);
    REQUIRE(Gem{Defs::gem::chrysoprase}.value() == gp50);
    REQUIRE(Gem{Defs::gem::citrine}.value() == gp50);
    REQUIRE(Gem{Defs::gem::jasper}.value() == gp50);
    REQUIRE(Gem{Defs::gem::moonstone}.value() == gp50);
    REQUIRE(Gem{Defs::gem::onyx}.value() == gp50);
    REQUIRE(Gem{Defs::gem::rock_crystal}.value() == gp50);
    REQUIRE(Gem{Defs::gem::sardonyx}.value() == gp50);
    REQUIRE(Gem{Defs::gem::smoky_quartz}.value() == gp50);
    REQUIRE(Gem{Defs::gem::star_rose_quartz}.value() == gp50);
    REQUIRE(Gem{Defs::gem::zircon}.value() == gp50);
    REQUIRE(Gem{Defs::gem::amber}.value() == gp100);
    REQUIRE(Gem{Defs::gem::alexandrite}.value() == gp100);
    REQUIRE(Gem{Defs::gem::amethyst}.value() == gp100);
    REQUIRE(Gem{Defs::gem::chrysoberyl}.value() == gp100);
    REQUIRE(Gem{Defs::gem::coral}.value() == gp100);
    REQUIRE(Gem{Defs::gem::jade}.value() == gp100);
    REQUIRE(Gem{Defs::gem::jet}.value() == gp100);
    REQUIRE(Gem{Defs::gem::tourmaline}.value() == gp100);
    REQUIRE(Gem{Defs::gem::aquamarine}.value() == gp500);
    REQUIRE(Gem{Defs::gem::peridot}.value() == gp500);
    REQUIRE(Gem{Defs::gem::topaz}.value() == gp500);
    REQUIRE(Gem{Defs::gem::black_opal}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::fire_opal}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::opal}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::oriental_amethyst}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::oriental_topaz}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::sapphire}.value() == gp1000);
    REQUIRE(Gem{Defs::gem::black_sapphire}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::diamon}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::emerald}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::jacinth}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::oriental_emerald}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::ruby}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::star_ruby}.value() == gp5000);
    REQUIRE(Gem{Defs::gem::star_sapphire}.value() == gp5000);
}

TEST_CASE("[TC-TREA.002] Gems with range value are valued accordingly", "[treasure]" )
{
    Coin gp100{Defs::coin::gold_piece, 100};
    Coin gp500{Defs::coin::gold_piece, 500};

    REQUIRE(Gem{Defs::gem::garnet}.value() >= gp100);
    REQUIRE(Gem{Defs::gem::garnet}.value() <= gp500);
    REQUIRE(Gem{Defs::gem::pearl}.value() >= gp100);
    REQUIRE(Gem{Defs::gem::pearl}.value() <= gp500);
    REQUIRE(Gem{Defs::gem::spinel}.value() >= gp100);
    REQUIRE(Gem{Defs::gem::spinel}.value() <= gp500);
}

TEST_CASE("[TC-TREA.003] Gems generated in sequence have progressive unique IDs", "[treasure]" )
{
    Gem g1{};
    Gem g2{};
    Gem g3{};
    REQUIRE(g1.uid() < g2.uid());
    REQUIRE(g2.uid() < g3.uid());
}

TEST_CASE("[TC-TREA.004] Objects of art value vary according to their probability", "[treasure]" )
{
    Coin gp10{Defs::coin::gold_piece, 10};
    Coin gp30{Defs::coin::gold_piece, 30};
    Coin gp100{Defs::coin::gold_piece, 100};
    Coin gp120{Defs::coin::gold_piece, 120};
    Coin gp180{Defs::coin::gold_piece, 180};
    Coin gp300{Defs::coin::gold_piece, 300};
    Coin gp400{Defs::coin::gold_piece, 400};
    Coin gp500{Defs::coin::gold_piece, 500};
    Coin gp600{Defs::coin::gold_piece, 600};
    Coin gp1000{Defs::coin::gold_piece, 1000};
    Coin gp1200{Defs::coin::gold_piece, 1200};
    Coin gp1800{Defs::coin::gold_piece, 1800};
    Coin gp2000{Defs::coin::gold_piece, 2000};
    Coin gp2400{Defs::coin::gold_piece, 2400};
    Coin gp3000{Defs::coin::gold_piece, 3000};
    Coin gp4000{Defs::coin::gold_piece, 4000};
    Coin gp6000{Defs::coin::gold_piece, 6000};
    Coin gp8000{Defs::coin::gold_piece, 8000};
    Coin gp12000{Defs::coin::gold_piece, 12000};

    for (int i=0; i<10; ++i)
    {
        ObjectOfArt obj{};
        if (obj.object_type() == 1)
        {
            REQUIRE(obj.value() >= gp10);
            REQUIRE(obj.value() <= gp100);
        }
        else if (obj.object_type() == 2)
        {
            REQUIRE(obj.value() >= gp30);
            REQUIRE(obj.value() <= gp180);
        }
        else if (obj.object_type() == 3)
        {
            REQUIRE(obj.value() >= gp100);
            REQUIRE(obj.value() <= gp600);
        }
        else if (obj.object_type() == 4)
        {
            REQUIRE(obj.value() >= gp100);
            REQUIRE(obj.value() <= gp1000);
        }
        else if (obj.object_type() == 5)
        {
            REQUIRE(obj.value() >= gp120);
            REQUIRE(obj.value() <= gp1200);
        }
        else if (obj.object_type() == 6)
        {
            REQUIRE(obj.value() >= gp300);
            REQUIRE(obj.value() <= gp1800);
        }
        else if (obj.object_type() == 7)
        {
            REQUIRE(obj.value() >= gp400);
            REQUIRE(obj.value() <= gp2400);
        }
        else if (obj.object_type() == 8)
        {
            REQUIRE(obj.value() >= gp500);
            REQUIRE(obj.value() <= gp3000);
        }
        else if (obj.object_type() == 9)
        {
            REQUIRE(obj.value() >= gp1000);
            REQUIRE(obj.value() <= gp4000);
        }
        else if (obj.object_type() == 10)
        {
            REQUIRE(obj.value() >= gp1000);
            REQUIRE(obj.value() <= gp6000);
        }
        else if (obj.object_type() == 11)
        {
            REQUIRE(obj.value() >= gp2000);
            REQUIRE(obj.value() <= gp8000);
        }
        else if (obj.object_type() == 12)
        {
            REQUIRE(obj.value() >= gp2000);
            REQUIRE(obj.value() <= gp12000);
        }
    }
}

TEST_CASE("[TC-TREA.005] Class A treasure always contains values if presence flag is forced", "[treasure]" )
{
    std::map<Defs::coin, std::pair<uint32_t, uint32_t>> coins
    {
        {Defs::coin::copper_piece, {1000, 3000}},
        {Defs::coin::silver_piece, {200, 2000}},
        {Defs::coin::gold_piece, {1000, 6000}},
    };
    std::pair<uint32_t, uint32_t> gpEpMinMax{300, 1800};
    std::pair<uint32_t, uint32_t> gemsMinMax{10, 40};
    std::pair<uint32_t, uint32_t> objectsOfArtMinMax{2, 12};
    uint32_t magicalsCount{3};
    
    OptionalRules::get_instance().option<bool>(Option::treasure_components_always_present) = true;
    Treasure t{Defs::treasure_class::a};

    for (auto& c : coins)
    {
        auto value = t.value(c.first);
        REQUIRE(value >= c.second.first);
        REQUIRE(value <= c.second.second);
    }
    
    if (!t.includes(Defs::coin::electrum_piece))
    {
        REQUIRE(t.includes(Defs::coin::platinum_piece));
        auto value = t.value(Defs::coin::platinum_piece);
        REQUIRE(value >= gpEpMinMax.first);
        REQUIRE(value <= gpEpMinMax.second);
    }
    else if (!t.includes(Defs::coin::platinum_piece))
    {
        REQUIRE(t.includes(Defs::coin::electrum_piece));
        auto value = t.value(Defs::coin::electrum_piece);
        REQUIRE(value >= gpEpMinMax.first);
        REQUIRE(value <= gpEpMinMax.second);
    }

    auto& gems = t.gems();
    REQUIRE(gems.size() >= gemsMinMax.first);
    REQUIRE(gems.size() <= gemsMinMax.second);

    auto& arts = t.objects_of_art();
    REQUIRE(arts.size() >= objectsOfArtMinMax.first);
    REQUIRE(arts.size() <= objectsOfArtMinMax.second);

    auto& magicals = t.magical_items();
    REQUIRE(magicals.size() == magicalsCount);

    OptionalRules::get_instance().option<bool>(Option::treasure_components_always_present) = false;
}

TEST_CASE("[TC-TREA.005] Treasures may contain appropriate values accorging to their class", "[treasure]" )
{
    std::map<Defs::treasure_class, std::map<Defs::coin, std::pair<uint32_t, uint32_t>>> coinsPerClass
    {
        {Defs::treasure_class::a,
            {
                {Defs::coin::copper_piece, {1000, 3000}},
                {Defs::coin::silver_piece, {200, 2000}},
                {Defs::coin::gold_piece, {1000, 6000}},
            }
        },
        {Defs::treasure_class::b,
            {
                {Defs::coin::copper_piece, {1000, 6000}},
                {Defs::coin::silver_piece, {1000, 3000}},
                {Defs::coin::gold_piece, {200, 2000}},
            }
        },
        {Defs::treasure_class::c,
            {
                {Defs::coin::copper_piece, {1000, 10000}},
                {Defs::coin::silver_piece, {1000, 6000}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::d,
            {
                {Defs::coin::copper_piece, {1000, 6000}},
                {Defs::coin::silver_piece, {1000, 10000}},
                {Defs::coin::gold_piece, {1000, 3000}},
            }
        },
        {Defs::treasure_class::e,
            {
                {Defs::coin::copper_piece, {1000, 6000}},
                {Defs::coin::silver_piece, {1000, 10000}},
                {Defs::coin::gold_piece, {1000, 4000}},
            }
        },
        {Defs::treasure_class::f,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {3000, 18000}},
                {Defs::coin::gold_piece, {1000, 6000}},
            }
        },
        {Defs::treasure_class::g,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {2000, 20000}},
            }
        },
        {Defs::treasure_class::h,
            {
                {Defs::coin::copper_piece, {3000, 18000}},
                {Defs::coin::silver_piece, {2000, 20000}},
                {Defs::coin::gold_piece, {2000, 20000}},
            }
        },
        {Defs::treasure_class::i,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::j,
            {
                {Defs::coin::copper_piece, {3, 24}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::k,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {3, 18}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::l,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::m,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {2, 8}},
            }
        },
        {Defs::treasure_class::n,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::o,
            {
                {Defs::coin::copper_piece, {10, 40}},
                {Defs::coin::silver_piece, {10, 30}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::p,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {10, 60}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::q,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::r,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {2, 20}},
            }
        },
        {Defs::treasure_class::s,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::t,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::u,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::v,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::w,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {5, 30}},
            }
        },
        {Defs::treasure_class::x,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {0, 0}},
            }
        },
        {Defs::treasure_class::y,
            {
                {Defs::coin::copper_piece, {0, 0}},
                {Defs::coin::silver_piece, {0, 0}},
                {Defs::coin::gold_piece, {200, 1200}},
            }
        },
        {Defs::treasure_class::z,
            {
                {Defs::coin::copper_piece, {100, 300}},
                {Defs::coin::silver_piece, {100, 400}},
                {Defs::coin::gold_piece, {100, 600}},
            }
        },
    };
    std::map<Defs::treasure_class, std::pair<uint32_t, uint32_t>> gpEpMinMax
    {
        {Defs::treasure_class::a, {300, 1800}},
        {Defs::treasure_class::b, {100, 1000}},
        {Defs::treasure_class::c, {100, 600}},
        {Defs::treasure_class::d, {100, 600}},
        {Defs::treasure_class::e, {300, 1800}},
        {Defs::treasure_class::f, {1000, 4000}},
        {Defs::treasure_class::g, {1000, 10000}},
        {Defs::treasure_class::h, {1000, 8000}},
        {Defs::treasure_class::i, {100, 600}},
        {Defs::treasure_class::j, {0, 0}},
        {Defs::treasure_class::k, {0, 0}},
        {Defs::treasure_class::l, {2, 12}},
        {Defs::treasure_class::m, {0, 0}},
        {Defs::treasure_class::n, {1, 6}},
        {Defs::treasure_class::o, {0, 0}},
        {Defs::treasure_class::p, {1, 20}},
        {Defs::treasure_class::q, {0, 0}},
        {Defs::treasure_class::r, {10, 60}},
        {Defs::treasure_class::s, {0, 0}},
        {Defs::treasure_class::t, {0, 0}},
        {Defs::treasure_class::u, {0, 0}},
        {Defs::treasure_class::v, {0, 0}},
        {Defs::treasure_class::w, {1, 8}},
        {Defs::treasure_class::x, {0, 0}},
        {Defs::treasure_class::y, {0, 0}},
        {Defs::treasure_class::z, {100, 400}},
    };
    std::map<Defs::treasure_class, std::pair<uint32_t, uint32_t>> gemsMinMax
    {
        {Defs::treasure_class::a, {10, 40}},
        {Defs::treasure_class::b, {1, 8}},
        {Defs::treasure_class::c, {1, 6}},
        {Defs::treasure_class::d, {1, 10}},
        {Defs::treasure_class::e, {1, 12}},
        {Defs::treasure_class::f, {2, 20}},
        {Defs::treasure_class::g, {3, 18}},
        {Defs::treasure_class::h, {3, 30}},
        {Defs::treasure_class::i, {2, 12}},
        {Defs::treasure_class::j, {0, 0}},
        {Defs::treasure_class::k, {0, 0}},
        {Defs::treasure_class::l, {0, 0}},
        {Defs::treasure_class::m, {0, 0}},
        {Defs::treasure_class::n, {0, 0}},
        {Defs::treasure_class::o, {0, 0}},
        {Defs::treasure_class::p, {0, 0}},
        {Defs::treasure_class::q, {1, 4}},
        {Defs::treasure_class::r, {2, 8}},
        {Defs::treasure_class::s, {0, 0}},
        {Defs::treasure_class::t, {0, 0}},
        {Defs::treasure_class::u, {2, 16}},
        {Defs::treasure_class::v, {0, 0}},
        {Defs::treasure_class::w, {2, 16}},
        {Defs::treasure_class::x, {0, 0}},
        {Defs::treasure_class::y, {0, 0}},
        {Defs::treasure_class::z, {1, 6}},
    };
    std::map<Defs::treasure_class, std::pair<uint32_t, uint32_t>> objectsOfArtMinMax
    {
        {Defs::treasure_class::a, {2, 12}},
        {Defs::treasure_class::b, {1, 4}},
        {Defs::treasure_class::c, {1, 3}},
        {Defs::treasure_class::d, {1, 6}},
        {Defs::treasure_class::e, {1, 6}},
        {Defs::treasure_class::f, {1, 8}},
        {Defs::treasure_class::g, {1, 6}},
        {Defs::treasure_class::h, {2, 20}},
        {Defs::treasure_class::i, {2, 8}},
        {Defs::treasure_class::j, {0, 0}},
        {Defs::treasure_class::k, {0, 0}},
        {Defs::treasure_class::l, {0, 0}},
        {Defs::treasure_class::m, {0, 0}},
        {Defs::treasure_class::n, {0, 0}},
        {Defs::treasure_class::o, {0, 0}},
        {Defs::treasure_class::p, {0, 0}},
        {Defs::treasure_class::q, {0, 0}},
        {Defs::treasure_class::r, {1, 3}},
        {Defs::treasure_class::s, {0, 0}},
        {Defs::treasure_class::t, {0, 0}},
        {Defs::treasure_class::u, {1, 6}},
        {Defs::treasure_class::v, {0, 0}},
        {Defs::treasure_class::w, {1, 8}},
        {Defs::treasure_class::x, {0, 0}},
        {Defs::treasure_class::y, {0, 0}},
        {Defs::treasure_class::z, {2, 12}},
    };
    std::map<Defs::treasure_class, std::pair<uint32_t, uint32_t>> magicalsCount
    {
        {Defs::treasure_class::a, {3, 3}},
        {Defs::treasure_class::b, {1, 1}},
        {Defs::treasure_class::c, {2, 2}},
        {Defs::treasure_class::d, {3, 3}},
        {Defs::treasure_class::e, {4, 4}},
        {Defs::treasure_class::f, {5, 5}},
        {Defs::treasure_class::g, {5, 5}},
        {Defs::treasure_class::h, {6, 6}},
        {Defs::treasure_class::i, {1, 1}},
        {Defs::treasure_class::j, {0, 0}},
        {Defs::treasure_class::k, {0, 0}},
        {Defs::treasure_class::l, {0, 0}},
        {Defs::treasure_class::m, {0, 0}},
        {Defs::treasure_class::n, {0, 0}},
        {Defs::treasure_class::o, {0, 0}},
        {Defs::treasure_class::p, {0, 0}},
        {Defs::treasure_class::q, {0, 0}},
        {Defs::treasure_class::r, {0, 0}},
        {Defs::treasure_class::s, {1, 8}},
        {Defs::treasure_class::t, {1, 4}},
        {Defs::treasure_class::u, {1, 1}},
        {Defs::treasure_class::v, {2, 2}},
        {Defs::treasure_class::w, {2, 2}},
        {Defs::treasure_class::x, {2, 2}},
        {Defs::treasure_class::y, {0, 0}},
        {Defs::treasure_class::z, {3, 3}},
    };
    
    for (auto& coins : coinsPerClass)
    {
        Treasure t{coins.first};

        for (auto& c : coins.second)
        {
            auto value = t.value(c.first);
            if (t.includes(c.first))
            {
                REQUIRE(value >= c.second.first);
                REQUIRE(value <= c.second.second);
            }
            else
            {
                REQUIRE(value == 0);
            }
        }

        if (t.includes_gems())
        {
            auto& gems = t.gems();
            REQUIRE(gems.size() >= gemsMinMax[coins.first].first);
            REQUIRE(gems.size() <= gemsMinMax[coins.first].second);
        }

        if (t.includes_objects_of_art())
        {
            auto& arts = t.objects_of_art();
            REQUIRE(arts.size() >= objectsOfArtMinMax[coins.first].first);
            REQUIRE(arts.size() <= objectsOfArtMinMax[coins.first].second);
        }

        if (t.includes_magicals())
        {
            auto& magicals = t.magical_items();
            REQUIRE(magicals.size() >= magicalsCount[coins.first].first);
            REQUIRE(magicals.size() <= magicalsCount[coins.first].second);
        }
    }
}