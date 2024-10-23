//#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <string>
#include <sstream>

#include <adndtk.h>

using namespace Adndtk;


TEST_CASE("[TC-VALI.001] Skill values can be any well-formed value between min and max values", "[validator]" )
{
    Adndtk::DataValidator validator{};

    for (auto skillValue = Const::min_skill_value; skillValue <= Const::max_skill_value; ++skillValue)
    {
        std::stringstream ss{};
        ss << skillValue;

        REQUIRE(validator.validate_skill(ss.str()));
    }
}

TEST_CASE("[TC-VALI.002] Skill values cannot be zero or negative", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_skill("0"));
    REQUIRE_FALSE(validator.validate_skill("-1"));
    REQUIRE_FALSE(validator.validate_skill("-2"));
    REQUIRE_FALSE(validator.validate_skill("-5"));
}

TEST_CASE("[TC-VALI.003] Skill values cannot be greater than maximum value", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_skill("26"));
    REQUIRE_FALSE(validator.validate_skill("30"));
}

TEST_CASE("[TC-VALI.004] Skill values cannot be greater than maximum value", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_skill("26"));
    REQUIRE_FALSE(validator.validate_skill("30"));
}

TEST_CASE("[TC-VALI.005] Skill values can contain spaces", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE(validator.validate_skill("17 "));
    REQUIRE(validator.validate_skill(" 15"));
    REQUIRE(validator.validate_skill(" 13 "));
    REQUIRE(validator.validate_skill("    14   "));
}

TEST_CASE("[TC-VALI.006] Strength values can contain spaces", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE(validator.validate_strength("17 "));
    REQUIRE(validator.validate_strength(" 15"));
    REQUIRE(validator.validate_strength(" 13 "));
    REQUIRE(validator.validate_strength("    14   "));
    REQUIRE(validator.validate_strength(" 18   "));
    REQUIRE(validator.validate_strength(" 18/81   "));
}

TEST_CASE("[TC-VALI.007] Exceptional strength values must be between 18/01 and 18/00", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_strength("18/-1"));
    REQUIRE(validator.validate_strength("18/09"));
    REQUIRE(validator.validate_strength("18/33"));
    REQUIRE(validator.validate_strength("18/61"));
    REQUIRE(validator.validate_strength("18/99"));
    REQUIRE(validator.validate_strength("18/00"));
    REQUIRE_FALSE(validator.validate_strength("18/100"));
    REQUIRE_FALSE(validator.validate_strength("18/110"));
    REQUIRE_FALSE(validator.validate_strength("18/234"));
}

TEST_CASE("[TC-VALI.008] When exceptional strength applies, strength value must equals 18", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_strength("12/78"));
    REQUIRE_FALSE(validator.validate_strength("17/21"));
    REQUIRE(validator.validate_strength("18/55"));
    REQUIRE_FALSE(validator.validate_strength("19/34"));
}

TEST_CASE("[TC-VALI.009] Generation methods can only be one of the valid values", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE(validator.validate_generation_method("standard"));
    REQUIRE(validator.validate_generation_method("best4"));
    REQUIRE(validator.validate_generation_method("best-each"));
    REQUIRE_FALSE(validator.validate_generation_method("best"));
    REQUIRE_FALSE(validator.validate_generation_method("standardd"));
}

TEST_CASE("[TC-VALI.010] Skill lists must be a sequence of valid skill values in standard order (STR, DEX, CON, INT, WIS, CHA)", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE(validator.validate_skills_list("11,12,14,15,13,12"));
    REQUIRE(validator.validate_skills_list("11, 12,14 , 15 ,13, 12"));
    REQUIRE(validator.validate_skills_list("  11,   12,14, 15,  13,12 "));
    REQUIRE(validator.validate_skills_list("11, 12, 14,      15,13,12 "));
    REQUIRE(validator.validate_skills_list("18, 12, 14,      15,13,12 "));
    REQUIRE(validator.validate_skills_list("18/35, 12, 14,      15,13,12 "));
}

TEST_CASE("[TC-VALI.011] Strength values must be at the first position", "[validator]" )
{
    Adndtk::DataValidator validator{};

    REQUIRE_FALSE(validator.validate_skills_list("11,18/12,14,15,13,12"));
    REQUIRE_FALSE(validator.validate_skills_list("11, 12,14 , 15 ,13, 18/82"));
    REQUIRE_FALSE(validator.validate_skills_list("  11,   12,18/34, 15,  13,12 "));
    REQUIRE_FALSE(validator.validate_skills_list("11, 12, 18/44,      15,13,12 "));
    REQUIRE_FALSE(validator.validate_skills_list("18, 12, 14,      18/75,13,12 "));
    REQUIRE_FALSE(validator.validate_skills_list("18/35, 12, 14,      15,18/43,12 "));
}

TEST_CASE("[TC-VALI.011] Skill list must contain exactly six values", "[validator]" )
{
    Adndtk::DataValidator validator{};

    auto list = validator.validate_skills_list("11,12,14,15,13,12");

    REQUIRE(list.has_value());
    REQUIRE(list.value().size() == 6);
}

TEST_CASE("[TC-VALI.012] Skill list must contain skill values in the order specified", "[validator]" )
{
    Adndtk::DataValidator validator{};

    auto list = validator.validate_skills_list("11,12,13,14,15,16");

    REQUIRE(list.has_value());
    REQUIRE(list.value().at(Adndtk::Defs::skill::strength) == 11);
    REQUIRE(list.value().at(Adndtk::Defs::skill::dexterity) == 12);
    REQUIRE(list.value().at(Adndtk::Defs::skill::constitution) == 13);
    REQUIRE(list.value().at(Adndtk::Defs::skill::intelligence) == 14);
    REQUIRE(list.value().at(Adndtk::Defs::skill::wisdom) == 15);
    REQUIRE(list.value().at(Adndtk::Defs::skill::charisma) == 16);
}

TEST_CASE("[TC-VALI.013] Classes can be validated by name", "[validator]" )
{
    Adndtk::DataValidator validator{};
    std::optional<Defs::character_class> classId{};
    
    classId = validator.validate_class("paladin");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::paladin);

    classId = validator.validate_class("ranger");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::ranger);

    classId = validator.validate_class("cleric");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::cleric);

    classId = validator.validate_class("druid");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::druid);

    classId = validator.validate_class("Fighter Mage Thief");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::fighter_mage_thief);
}

TEST_CASE("[TC-VALI.014] Classes can be validated by capitalised name", "[validator]" )
{
    Adndtk::DataValidator validator{};
    std::optional<Defs::character_class> classId{};

    classId = validator.validate_class("Paladin");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::paladin);

    classId = validator.validate_class("Ranger");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::ranger);

    classId = validator.validate_class("Cleric");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::cleric);

    classId = validator.validate_class("Druid");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::druid);

    classId = validator.validate_class("fighter mage thief");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::fighter_mage_thief);
}

TEST_CASE("[TC-VALI.015] Classes can be validated by acronym", "[validator]" )
{
    Adndtk::DataValidator validator{};
    std::optional<Defs::character_class> classId{};

    classId = validator.validate_class("P");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::paladin);

    classId = validator.validate_class("R");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::ranger);

    classId = validator.validate_class("C");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::cleric);

    classId = validator.validate_class("D");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::druid);

    classId = validator.validate_class("F/M/T");
    REQUIRE(classId.has_value());
    REQUIRE(classId.value() == Adndtk::Defs::character_class::fighter_mage_thief);
}