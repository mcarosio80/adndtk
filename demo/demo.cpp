#include <iostream>
#include <adndtk.h>

using namespace Adndtk;

void demo_cyclopedia();
void demo_dice();
void demo_skills();
void demo_experience();
void demo_hp();
void demo_options();
void demo_thaco();
void demo_turn_undead();
void demo_character();

void print_character(const Character&);

int main(int argc, char **argv)
{
    std::cout << Metadata::library_name
                << " " << Metadata::version
                << " by " << Metadata::author << "\n";
    std::cout << "Last library commit: " << Metadata::last_commit_date << "(" << Metadata::last_commit_hash <<")" << "\n";
    
    std::cout << "\nDESCRIPTION:\n" << Metadata::desc << "\n\n";

    demo_character();
    
    return 0;
}

void print_character(Character& chr)
{
    std::cout << "Character data sheet\n";
    std::cout << "--------------------------------------\n\n";

    std::cout << "Name:\t" << chr.name() << "\n";
    std::cout << "\tAge:\t" << chr.age() << "\n";
    std::cout << "\tHeight:\t" << chr.height() << " cm\n";
    std::cout << "\tWeight:\t" << chr.weight() << " Kg\n";
    std::cout << "\tGross weight:\t" << chr.total_weight() << " Kg\n";

    auto strScore = chr.strength();
    auto dexScore = chr.dexterity();
    auto conScore = chr.constitution();
    auto intScore = chr.intelligence();
    auto wisScore = chr.wisdom();
    auto chaScore = chr.charisma();
    short totalScore = strScore + dexScore + conScore + intScore + wisScore + chaScore;

    std::cout << "Str:\t" << strScore << "\n";
    std::cout << "Dex:\t" << dexScore << "\n";
    std::cout << "Con:\t" << conScore << "\n";
    std::cout << "Int:\t" << intScore << "\n";
    std::cout << "Wis:\t" << wisScore << "\n";
    std::cout << "Cha:\t" << chaScore << "\n";
    std::cout << "Total:\t" << totalScore << "\n";

    auto xps = chr.experience();
    std::cout << "XP: ";
    for (auto& xp : xps)
    {
        std::cout << xps.level(xp.first) << "(" << xps.xp(xp.first) << ") ";
    }
    std::cout << "\n";

    std::cout << "HP: " << chr.hp() << "/" << chr.total_hp() << "  (" << chr.health()*100.0 << "%)" << "\n";
    std::cout << "Alive: " << ((chr.is_alive()) ? "yes" : "no") << "\n";
}

void demo_character()
{
    std::string chrName{"Fingolfin"};
    Defs::character_class chrClass{Defs::character_class::fighter_mage_thief};
    Defs::race chrRace{Defs::race::half_elf};

    OptionalRules::get_instance().option<int>(Option::skills_generation_method) = static_cast<int>(SkillGenerationMethod::best_of_four);
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;

    Character chr{chrName, chrClass, chrRace, Defs::moral_alignment::true_neutral, Defs::sex::male};

    chr.gain_xp(1900);
    print_character(chr);
    chr.gain_xp(650, Defs::character_class::thief);
    print_character(chr);
    chr.lose_xp(300);
    print_character(chr);
    chr.lose_xp(300, Defs::character_class::fighter);
    print_character(chr);

    chr.gain_level(1);
    print_character(chr);
    chr.gain_level(1, Defs::character_class::mage);
    print_character(chr);
    chr.lose_level(1, Defs::character_class::mage);
    print_character(chr);
    chr.lose_level(1);
    print_character(chr);

    chr.wound(3);
    print_character(chr);
    chr.gain_level(1);
    print_character(chr);
    chr.lose_level(1);
    print_character(chr);
}

void demo_cyclopedia()
{
    Query queryId = Query::select_coin;

    auto res = Cyclopedia::get_instance().exec_prepared_statement<Defs::coin>(queryId, Defs::coin::gold_piece);
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
    Experience exp{cls, Defs::race::human};
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

void demo_hp()
{
    Defs::character_class cls = Defs::character_class::fighter_mage_thief;
    Defs::character_class clsF = Defs::character_class::fighter;
    Defs::character_class clsM = Defs::character_class::mage;
    Defs::character_class clsT = Defs::character_class::thief;

    HitPoints hp{cls};
    hp.increase(clsF);
    hp.increase(clsF);
    hp.increase(clsF);

    std::cout << hp.total() << std::endl;
    std::cout << hp.current() << std::endl;

    hp.shrink(clsF);

    std::cout << hp.total() << std::endl;
    std::cout << hp.current() << std::endl;

    HP wounds = 1;
    hp -= wounds;
    std::cout << hp.total() << std::endl;
    std::cout << hp.current() << std::endl;

    hp.shrink(clsF);
    std::cout << hp.total() << std::endl;
    std::cout << hp.current() << std::endl;

    hp.increase(clsF);
    std::cout << hp.total() << std::endl;
    std::cout << hp.current() << std::endl;
}

void demo_options()
{
    std::cout << "Max score for HD = true" << std::endl;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = true;
    HitPoints hp{Defs::character_class::fighter};

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;

    std::cout << "Max score for HD = false" << std::endl;
    OptionalRules::get_instance().option<bool>(Option::max_score_for_hd) = false;

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;

    hp.increase(Defs::character_class::fighter);
    std::cout << hp.current() << std::endl;
}

void demo_thaco()
{
    Defs::character_class_type typ = Defs::character_class_type::wizard;
    Thaco t{typ};

    std::cout << "THAC0[W/1] = " << t.get(1) << std::endl;
}

void demo_turn_undead()
{
    std::pair<Defs::turn_effect, std::optional<short>> res = TurnUndead::get_instance().get(15, Defs::turnable::skeleton);
    std::cout << "Result: " << static_cast<int>(res.first) << std::endl;
    //std::cout << "Result: " << res.second.value() << std::endl;
    std::cout << "Result: " << std::boolalpha << res.second.has_value() << std::endl;
}