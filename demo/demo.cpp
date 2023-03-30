#include <iostream>
#include <adndtk.h>

using namespace Adndtk;

void demo_cyclopedia();
void demo_dice();
void demo_skills();
void demo_experience();

int main(int argc, char **argv)
{
    std::cout << Metadata::library_name
                << " " << Metadata::version
                << " by " << Metadata::author << "\n";
    std::cout << "Last library commit: " << Metadata::last_commit_date << "(" << Metadata::last_commit_hash <<")" << "\n";
    
    std::cout << "\nDESCRIPTION:\n" << Metadata::desc << "\n\n";

    //demo_cyclopedia();
    //demo_dice();
    //demo_skills();
    demo_experience();

    return 0;
}

void demo_cyclopedia()
{
    Query queryId = Query::select_coin;
    int param = static_cast<int>(Defs::coin::gold_piece);

    auto res = Cyclopedia::get_instance().exec_prepared_statement<int>(queryId, param);
    for (auto& r : res)
    {
        std::cout << r << "\n";
    }
}

void demo_dice()
{
    Die d{Defs::die::d8};
    int numRoll = 3;

    for (int i=0; i<6; ++i)
    {
        int result = d.roll(numRoll);
        std::cout << "Result" << "[" << i+1 << "] of " << numRoll << d << ": " << result << "\n";
    }

    std::string expr{"4d4+1"};
    for (int i=0; i<6; ++i)
    {
        int result = Die::roll(expr.c_str());
        std::cout << "Result[" << i+1 << "] of " << expr << ": " << result << "\n";
    }

    Die d6;
    for (int i=0; i<6; ++i)
    {
        std::cout << "Result" << "[" << i+1 << "] of 1" << d6 << ": " << static_cast<int>(d6) << "\n";
    }

    std::string malformedExpr{"-3d4-"};
    try
    {
        int res = Die::roll(malformedExpr.c_str());
        std::cout << "Result of " << malformedExpr << ": " << res << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::string expr2{"3d5"};
    int res = Die::roll(expr2.c_str());
    std::cout << "Result of " << expr2 << ": " << res << "\n";

    std::string expr3{"4  -  10"};
    int res3 = Die::roll(expr3.c_str());
    std::cout << "Result of " << expr3 << ": " << res3 << "\n";
}

void demo_skills()
{
    {
        Defs::character_class cls = Defs::character_class::bard;
        Defs::race race = Defs::race::half_elf;

        SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
        SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
        SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
        SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
        SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
        SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
        std::cout << str << ", " << dex << ", " << con << ", " << inl << ", " << wis << ", " << cha << std::endl;
    }
    {
        Defs::character_class cls = Defs::character_class::mage;
        Defs::race race = Defs::race::elf;

        SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
        SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
        SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
        SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
        SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
        SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
        std::cout << str << ", " << dex << ", " << con << ", " << inl << ", " << wis << ", " << cha << std::endl;
    }
    {
        Defs::character_class cls = Defs::character_class::fighter_mage;
        Defs::race race = Defs::race::elf;

        SkillValue str = SkillCreator::create(Defs::skill::strength, cls, race);
        SkillValue dex = SkillCreator::create(Defs::skill::dexterity, cls, race);
        SkillValue con = SkillCreator::create(Defs::skill::constitution, cls, race);
        SkillValue inl = SkillCreator::create(Defs::skill::intelligence, cls, race);
        SkillValue wis = SkillCreator::create(Defs::skill::wisdom, cls, race);
        SkillValue cha = SkillCreator::create(Defs::skill::charisma, cls, race);
        std::cout << str << ", " << dex << ", " << con << ", " << inl << ", " << wis << ", " << cha << std::endl;
    }
}

void demo_experience()
{
    const AdvancementTable& advTable = Cyclopedia::get_instance().advancement_table();

    Defs::character_class cls = Defs::character_class::druid;
    for (ExperienceLevel lvl = 1; lvl<=25; ++lvl)
    {
        auto points = advTable.get_xp_for_level(cls, lvl);
        std::cout << "ADV[" << lvl << "] -> " << points << "\n";
    }

    ExperienceLevel lvl{};
    XP points{0};

    points = 0;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 1000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 2000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 2250;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 3000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 4000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 9000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 32400;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 1000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 2000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 3000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 4000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 5000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 5500000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

    points = 6000000;
    lvl = advTable.get_level(cls, points);
    std::cout << points << " -> " << lvl << "\n";

 
    auto cbk = [&](const Defs::character_class& cls, const XPChangeType& chgType,
                            const XP& prevXP, const ExperienceLevel& prevLvl,
                            const XP& newXP, const ExperienceLevel& newLvl) -> void
    {
        std::string evtType{};
        switch (chgType)
        {
            case XPChangeType::level_down:
                evtType = "Level down"; 
                break;
            case XPChangeType::level_up:
                evtType = "Level up"; 
                break;
            case XPChangeType::level_zero:
                evtType = "Level zero"; 
                break;
            case XPChangeType::death:
                evtType = "Death"; 
                break;
        };
        std::cout << evtType << ": from level " << prevLvl << " (" << prevXP << ") to " << newLvl << " (" << newXP << ")\n"; 
    };


    cls = Defs::character_class::paladin;
    Experience exp{cls};
    exp += cbk;
    exp += 1000;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 1000;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 250;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 2250;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 4499;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 1;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp -= 1;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp += 1;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

    exp -= 6000;
    std::cout << "XP: " << exp.xp() << ", level " << exp.level() << "\n";

}