#include <iostream>
#include <adndtk.h>

using namespace Adndtk;

void demo_cyclopedia();
void demo_dice();
void demo_skills();

int main(int argc, char **argv)
{
    std::cout << Metadata::library_name
                << " " << Metadata::version
                << " by " << Metadata::author << "\n";
    std::cout << "Last library commit: " << Metadata::last_commit_date << "(" << Metadata::last_commit_hash <<")" << "\n";
    
    std::cout << "\nDESCRIPTION:\n" << Metadata::desc << "\n\n";

    demo_cyclopedia();
    demo_dice();
    demo_skills();

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