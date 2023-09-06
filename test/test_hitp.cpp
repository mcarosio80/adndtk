//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>

#include <adndtk.h>

using namespace Adndtk;

TEST_CASE("[TC-HITP.001] Increasing length of HP sequences generates new HD values", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};
    hp.set_constitution(13);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.increase(clsT);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();
}

TEST_CASE("[TC-HITP.002] Shrink of HP sequences reduces HP values", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};
    hp.set_constitution(13);

    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsT);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
    REQUIRE(hp.total() > previousTotal);
    previousTotal = hp.total();

    hp.shrink(clsF);
    REQUIRE(hp.total() < previousTotal);
    previousTotal = hp.total();

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 2);
}

TEST_CASE("[TC-HITP.003] Shrink and increase of HP sequence keep the previous HD generated value", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HP previousTotal{0};
    HP currentTotal{0};
    HitPoints hp{cls};
    hp.set_constitution(13);

    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsF);

    REQUIRE(hp.level(clsF) == 4);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    previousTotal = hp.total();

    hp.shrink(clsF);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() < previousTotal);

    hp.increase(clsF);
    REQUIRE(hp.level(clsF) == 4);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);
    REQUIRE(hp.total() == previousTotal);
}

TEST_CASE("[TC-HITP.004] Shrink and increase of HP sequence change the current HP score", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};
    hp.set_constitution(13);

    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsF);

    REQUIRE(hp.total() == hp.current());

    hp.shrink(clsF);
    REQUIRE(hp.total() == hp.current());

    HP wounds = 1;
    hp -= wounds;
    REQUIRE(hp.total() - wounds == hp.current());
    hp.shrink(clsF);
    REQUIRE(hp.total() - wounds == hp.current());
    hp.increase(clsF);
    REQUIRE(hp.total() - wounds == hp.current());
}

TEST_CASE("[TC-HITP.005] Dice roll yield at least 1 HP for multiclass", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsM = Defs::character_class::mage;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};

    HP prevHP = hp.total();
    hp.increase(clsM);
    REQUIRE(hp.total() > prevHP);
    
    prevHP = hp.total();
    hp.increase(clsM);
    REQUIRE(hp.total() > prevHP);
}

TEST_CASE("[TC-HITP.006] Level zero characters cannot advance", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};
    hp.set_constitution(13);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);

    hp.shrink(clsM);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 1);

    REQUIRE_NOTHROW(hp.increase(clsT));

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);

    REQUIRE_NOTHROW(hp.increase(clsF));

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);

    REQUIRE_THROWS(hp.increase(clsM));

    REQUIRE(hp.level(clsF) == 2);
    REQUIRE(hp.level(clsM) == 0);
    REQUIRE(hp.level(clsT) == 2);
}

TEST_CASE("[TC-HITP.007] Level adjustments receive callbacks", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};
    hp.set_constitution(13);

    HPChangeType chgType{};
    HP prevHP{};
    HP nextHP{};
    OnHPChange cbk = [&] (const HPChangeType& ct, const HP& prev, const HP& next) -> void
    {
        chgType = ct;
        prevHP = prev;
        nextHP = next;
    };
    hp += cbk;

    HP currentHP = hp.current();
    HP hpChange{};
    
    hpChange = 1;
    hp -= hpChange;
    REQUIRE(chgType == HPChangeType::wounded);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP - hpChange);
    currentHP = nextHP;
    
    hpChange = 1;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::healed);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP + hpChange);
    currentHP = nextHP;
    
    hpChange = 0;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::none);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP);
    currentHP = nextHP;
    
    hpChange = 10;
    hp += hpChange;
    REQUIRE(chgType == HPChangeType::none);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == currentHP);
    currentHP = nextHP;
    
    hpChange = 100;
    hp -= hpChange;
    REQUIRE(chgType == HPChangeType::dead);
    REQUIRE(prevHP == currentHP);
    REQUIRE(nextHP == 0);
}

TEST_CASE("[TC-HITP.008] Max score for HD ensures maximum HP score", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};
    hp.set_constitution(13);

    REQUIRE(hp.total() == 10);
    hp.increase(cls, 3);

    REQUIRE(hp.total() == 40);
    REQUIRE(hp.current() == 40);
    REQUIRE(hp.level(cls) == 4);
}

TEST_CASE("[TC-HITP.009] Level for multiclass is the highest level among the classes", "[HP]" )
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    HitPoints hp{cls};
    hp.set_constitution(13);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 1);

    hp.increase(clsT, 3);

    REQUIRE(hp.level(clsF) == 1);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 4);

    REQUIRE(hp.level() == 4);

    hp.increase(clsF, 2);

    REQUIRE(hp.level(clsF) == 3);
    REQUIRE(hp.level(clsM) == 1);
    REQUIRE(hp.level(clsT) == 4);

    REQUIRE(hp.level() == 4);
}