//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <defs.h>
#include <battleground.h>

using namespace Adndtk;

constexpr auto battleFieldName{"Battle of the Bones"};
constexpr auto whiteWalkers{"The White Walkers"};
constexpr auto nightWatch{"Night's Watch"};

TEST_CASE("[TC-BATL.001] When a new battlefield is requested, a brand new is created", "[battleground]" )
{
    REQUIRE(Battleground::get_instance().size() == 0);
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);
    REQUIRE(Battleground::get_instance().size() == 1);
    REQUIRE(bf.get_name() == battleFieldName);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.002] When a new battlefield is try-requested, none is created", "[battleground]" )
{
    REQUIRE(Battleground::get_instance().size() == 0);
    auto bf = Battleground::get_instance().try_get_battlefield(battleFieldName);
    REQUIRE_FALSE(bf.has_value());
    REQUIRE(Battleground::get_instance().size() == 0);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.003] When a new battlefield is added and then try.requested, the new one is returned", "[battleground]" )
{
    REQUIRE(Battleground::get_instance().size() == 0);
    Battleground::get_instance().add_battlefield(battleFieldName);
    auto bf = Battleground::get_instance().try_get_battlefield(battleFieldName);
    REQUIRE(bf.has_value());
    REQUIRE(Battleground::get_instance().size() == 1);
    REQUIRE(bf.value().get_name() == battleFieldName);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.004] New battlefield is empty", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);
    REQUIRE(bf.count_parties() == 0);
    REQUIRE(bf.count_avatars() == 0);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.005] Parties' creation increases the battlefield's size", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);
    REQUIRE(bf.count_parties() == 0);
    REQUIRE(bf.count_avatars() == 0);

    auto& ww = bf.get_party(whiteWalkers);
    REQUIRE(ww.get_name() == whiteWalkers);
    auto& nw = bf.get_party(nightWatch);
    REQUIRE(nw.get_name() == nightWatch);

    REQUIRE(bf.count_parties() == 2);
    REQUIRE(bf.count_avatars() == 0);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.006] Newly created Parties have zero size", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& p = bf.get_party(whiteWalkers);
    REQUIRE(p.size() == 0);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.007] Multiple calls to get_party return references to the same object", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);
    auto& p1 = bf.get_party(whiteWalkers);
    p1.add_monster(Defs::monster::deepspawn);
    p1.add_monsters<2>(Defs::monster::feyr);
    p1.add_monsters<4>(Defs::monster::gray_slaad);

    auto& p2 = bf.get_party(whiteWalkers);
    REQUIRE(p1.size() == p2.size());

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.008] Parties with members have non-zero size", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& p = bf.get_party(whiteWalkers);

    p.add_monster(Defs::monster::zombie_lord);
    p.add_monsters<2>(Defs::monster::zombie);
    bf.add_monster(Defs::monster::zombie_wolf, p.get_name(), {1.0,0.0}, 0.5);
    bf.add_monsters<3>(Defs::monster::zombie, p.get_name(), {{{-1.0,-1.0}, {-1.0,0.0}, {0.0,-1.0}}}, {{0.5,0.5,0.5}});
    REQUIRE(p.size() == 7);
    REQUIRE(p.count_monsters() == 7);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.009] Select returns filtered elements only", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& p = bf.get_party(whiteWalkers);

    bf.add_monsters<3>(Defs::monster::giant_frost, whiteWalkers, {{{-1.0,-1.0}, {-1.0,0.0}, {0.0,-1.0}}}, {{0.5,0.5,0.5}});
    bf.add_monsters<7>(Defs::monster::zombie, whiteWalkers, {{{-2.0,-2.0}, {-2.0,4.0}, {-2.0,6.0}, {-2.0,2.0}, {-2.0,3.0}, {-3.0,3.0}, {-3.0,4.0}}},
                    {{0.5,0.5,0.5,0.5,0.5,0.5,0.5}});
    bf.add_monsters<6>(Defs::monster::skeleton, whiteWalkers, {{{1.0,-1.0}, {1.0,-2.0}, {1.0,-3.0}, {2.0,-2.0}, {2.0,-3.0}, {3.0,-3.0}}},
                    {{0.5,0.5,0.5,0.5,0.5,0.5}});
    bf.add_monsters<4>(Defs::monster::gnoll, whiteWalkers, {{{1.0,1.0}, {1.0,0.0}, {0.0,1.0}, {0.0,2.0}}},
                    {{0.5,0.5,0.5,0.5}});
    REQUIRE(p.size() == 20);
    REQUIRE(p.count_monsters() == 20);

    auto filterUndead = [](const Monster& m){ return m.is_undead(); };
    auto undeads = bf.select<Monster>(filterUndead);
    REQUIRE(undeads.size() == 13);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.010] Filtered elements are writable references to the objects actually stored into the battlefield", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    constexpr auto noName = "no-name";
    constexpr auto nightKingName = "The Night King";
    auto& p = bf.get_party(whiteWalkers);
    bf.add_monster(Defs::monster::zombie_lord, whiteWalkers, Battlefield::origin, Battlefield::defaultRadius, std::nullopt, noName);
    REQUIRE(p.size() > 0);

    auto filterAll = [](const Monster& m){ return true; };

    {
        auto allMonsters1 = bf.select<Monster>(filterAll);
        auto& m1 = allMonsters1[0];
        REQUIRE(m1.avatar.get().get_unique_name() == noName);
        m1.avatar.get().set_unique_name(nightKingName);
    }

    {
        auto allMonsters2 = bf.select<Monster>(filterAll);
        auto& m2 = allMonsters2[0];
        REQUIRE(m2.avatar.get().get_unique_name() == nightKingName);
    }

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.011] Avatar with overlapping coordinates collide", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& p = bf.get_party(whiteWalkers);
    bf.add_monster(Defs::monster::skeleton, whiteWalkers, Battlefield::origin);
    REQUIRE_THROWS(bf.add_monster(Defs::monster::su_monster, whiteWalkers, Battlefield::origin, 0.4));

    REQUIRE(p.size() == 1);
    REQUIRE(bf.count_avatars() == 1);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.012] Avatar too close collide", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& p = bf.get_party(whiteWalkers);
    bf.add_monster(Defs::monster::skeleton, whiteWalkers, Battlefield::origin);
    REQUIRE_THROWS(bf.add_monster(Defs::monster::su_monster, whiteWalkers, {0.3,0.3}, 0.4));

    REQUIRE(p.size() == 1);
    REQUIRE(bf.count_avatars() == 1);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.013] Parties selection returns expected results", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    auto& ww = bf.get_party(whiteWalkers);
    auto& nw = bf.get_party(nightWatch);

    bf.add_monsters<6>(Defs::monster::wight, whiteWalkers, {{{1.0,-1.0}, {1.0,-2.0}, {1.0,-3.0}, {2.0,-2.0}, {2.0,-3.0}, {3.0,-3.0}}},
                    {{0.5,0.5,0.5,0.5,0.5,0.5}});
    REQUIRE(ww.size() == 6);
    REQUIRE(bf.count_avatars() == 6);

    bf.add_monsters<5>(Defs::monster::wolf_winter, nightWatch, {{{2.0,-2.0}, {2.0,-4.0}, {2.0,-6.0}, {4.0,-4.0}, {4.0,-6.0}}},
                    {{0.5,0.5,0.5,0.5,0.5}});
    REQUIRE(nw.size() == 5);
    REQUIRE(bf.count_avatars() == 11);
    
    auto partiesWithUndeads = [](const Party& p)
    {
        for (auto& m : p.monsters())
        {
            if (m.avatar.get().is_undead()) return true;
        }
        return false;
    };
    auto undeadParties = bf.select(partiesWithUndeads);
    REQUIRE(undeadParties.size() == 1);

    const auto& p = undeadParties[0].get();
    REQUIRE(p.get_name() == whiteWalkers);
    REQUIRE(p.size() == 6);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.014] Each avatar takes part in the turn according to initiative roll", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    bf.add_monsters<3>(Defs::monster::ghost, whiteWalkers, {{{1.0,-1.0}, {1.0,-2.0}, {1.0,-3.0}}},
                    {{0.5,0.5,0.5}});

    bf.add_character("Jon Snow", Defs::character_class::ranger, Defs::race::human,
            Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::chauntea, {2.0,-2.0}, 0.5);
    bf.add_character("Samwell Tarly", Defs::character_class::bard, Defs::race::human,
            Defs::moral_alignment::lawful_neutral, Defs::sex::male, std::nullopt, {2.0,-4.0}, 0.5);
    bf.add_character("Tyrion Lannister", Defs::character_class::fighter, Defs::race::dwarf,
            Defs::moral_alignment::chaotic_good, Defs::sex::male, std::nullopt, {2.0,-6.0}, 0.5);
            
    bf.step();
    auto& turn = bf.battle_turn();

    REQUIRE(turn.size() == 6);
    for (size_t i{1}; i<turn.size(); ++i)
    {
        REQUIRE(turn[i-1].first <= turn[i].first);
    }

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.015] Each avatar takes part in the turn only once", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    bf.add_monsters<3>(Defs::monster::ghost, whiteWalkers, {{{1.0,-1.0}, {1.0,-2.0}, {1.0,-3.0}}},
                    {{0.5,0.5,0.5}});

    bf.add_character("Jon Snow", Defs::character_class::ranger, Defs::race::human,
            Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::chauntea, {2.0,-2.0}, 0.5);
    bf.add_character("Samwell Tarly", Defs::character_class::bard, Defs::race::human,
            Defs::moral_alignment::lawful_neutral, Defs::sex::male, std::nullopt, {2.0,-4.0}, 0.5);
    bf.add_character("Tyrion Lannister", Defs::character_class::fighter, Defs::race::dwarf,
            Defs::moral_alignment::chaotic_good, Defs::sex::male, std::nullopt, {2.0,-6.0}, 0.5);
            
    bf.step();
    auto& turn = bf.battle_turn();

    short ghosts{0};
    short jon{0};
    short samwell{0};
    short tyrion{0};

    std::for_each(turn.begin(), turn.end(), [&](const Battlefield::BattleTurn& t) {
        if (t.second.get().avatar_type() == Avatar::Type::Monster) ++ghosts;
        else if (t.second.get().name() == "Jon Snow") ++jon;
        else if (t.second.get().name() == "Samwell Tarly") ++samwell;
        else if (t.second.get().name() == "Tyrion Lannister") ++tyrion;
    });

    REQUIRE(ghosts == 3);
    REQUIRE(jon == 1);
    REQUIRE(samwell == 1);
    REQUIRE(tyrion == 1);

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.016] Moving one step forward may change the turn list", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    // auto& ww = bf.get_party(whiteWalkers);
    // auto& nw = bf.get_party(nightWatch);

    bf.add_monster(Defs::monster::ghost, {1.0, -1.0}, 0.5, std::nullopt, "Ghost 01");
    bf.add_monster(Defs::monster::ghost, {1.0, -2.0}, 0.5, std::nullopt, "Ghost 02"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -3.0}, 0.5, std::nullopt, "Ghost 03"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -4.0}, 0.5, std::nullopt, "Ghost 04"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -5.0}, 0.5, std::nullopt, "Ghost 05"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -6.0}, 0.5, std::nullopt, "Ghost 06"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -7.0}, 0.5, std::nullopt, "Ghost 07"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -8.0}, 0.5, std::nullopt, "Ghost 08"); 
    bf.add_monster(Defs::monster::ghost, {1.0, -9.0}, 0.5, std::nullopt, "Ghost 09"); 
    bf.add_monster(Defs::monster::ghost, {1.0,-10.0}, 0.5, std::nullopt, "Ghost 10"); 

    bf.add_character("Jon Snow", Defs::character_class::ranger, Defs::race::human,
            Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::chauntea, {2.0,-2.0}, 0.5);
    bf.add_character("Samwell Tarly", Defs::character_class::bard, Defs::race::human,
            Defs::moral_alignment::lawful_neutral, Defs::sex::male, std::nullopt, {2.0,-4.0}, 0.5);
            
    bf.step();
    auto turn1 = bf.battle_turn();
    REQUIRE(turn1.size() == 12);
       
    bf.step();
    auto turn2 = bf.battle_turn();
    REQUIRE(turn2.size() == 12);

    auto fEquals = [](const Battlefield::BattleTurn& t1, const Battlefield::BattleTurn& t2)
    {
        return t1.first == t2.first
                && t1.second.get().avatar_type() == t2.second.get().avatar_type()
                && t1.second.get().name() == t2.second.get().name();
    };
    REQUIRE_FALSE(std::equal(turn1.begin(), turn1.end(), turn2.begin(), fEquals));

    Battleground::get_instance().clear();
}

TEST_CASE("[TC-BATL.017] Standalone Character objects can be added to the battlefield", "[battleground]" )
{
    auto& bf = Battleground::get_instance().get_battlefield(battleFieldName);

    Monster ghost{Defs::monster::ghost, "Angus Roy"};
    Monster zombie{Defs::monster::zombie_lord, "The Night King"};

    Character jon{"Jon Snow", Defs::character_class::ranger, Defs::race::human,
            Defs::moral_alignment::neutral_good, Defs::sex::male, Defs::deity::chauntea};
    Character sam{"Samwell Tarly", Defs::character_class::bard, Defs::race::human,
            Defs::moral_alignment::lawful_neutral, Defs::sex::male, std::nullopt};
            
    bf.add_character(std::move(jon), nightWatch, {2.0,-2.0}, 0.5);
    bf.add_character(std::move(sam), nightWatch, {2.0,-4.0}, 0.5);

    bf.add_monster(std::move(ghost), whiteWalkers, {1.0, -1.0}, 0.5);
    bf.add_monster(std::move(zombie), whiteWalkers, {1.0, -2.0}, 0.5);

    REQUIRE(bf.count_parties() == 2);
    REQUIRE(bf.count_avatars() == 4);

    Battleground::get_instance().clear();
}