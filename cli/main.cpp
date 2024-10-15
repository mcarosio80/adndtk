#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <set>
#include <map>

#include <adndtk.h>
#include <cli_options.h>
#include <cli_validator.h>
#include <character_generator.h>

template<typename _RetType = std::string>
_RetType prompt(const std::string& prompt)
{
    bool continuePrompt{false};
    _RetType ret{};
    do
    {
        std::cout << prompt << ": ";
        std::string input{};
        getline(std::cin, input);
        std::stringstream ss{input};
        try{
            ss >> ret;
            continuePrompt = false;
        }
        catch(...)
        {
            std::cerr << "Invalid input provided '" << input << "'\n";
            continuePrompt = true;
        }
    }
    while (continuePrompt);
    return ret;
}

void generate_character(const Adndtk::SkillGenerationMethod& method);

int main(int argc, char** argv)
{
    std::cout << "AdndTK CLI tool\n\n";
    CliTools::CliOptions cliOpts{argc, argv};
    CliTools::AdndtkCliValidator validator{};

    if (cliOpts.has_option(CliTools::Option::help)) {
        std::cout << cliOpts.get_help() << "\n";
        return 0;
    }

    if (cliOpts.has_option(CliTools::Option::generate)) {
        std::cout << "Entering automatic character generation..." << "\n\n";
        auto argVal = cliOpts.get_option<std::string>(CliTools::Option::generate);
        if (argVal.has_value())
        {
            auto genMethod = validator.validate_generation_method(argVal.value());
            if (genMethod.has_value())
            {
                generate_character(genMethod.value());
            }
            else
            {
                std::cerr << "Invalid argument provided" << "\n\n";
            }
        }
        else
        {
            std::cerr << "Missing argument for character generator" << "\n\n";
        }
        return 0;
    }

    return 0;
}

void generate_character(const Adndtk::SkillGenerationMethod& method)
{
    std::map<Adndtk::Defs::skill, Adndtk::SkillValue> skills{};
    bool accept{false};
    do
    {
        std::cout << "Generating stats...\n";
        for (auto skl : {Adndtk::Defs::skill::strength, Adndtk::Defs::skill::dexterity, Adndtk::Defs::skill::constitution,
                        Adndtk::Defs::skill::intelligence, Adndtk::Defs::skill::wisdom, Adndtk::Defs::skill::charisma})
        {
            skills[skl] = Adndtk::SkillCreator::create(skl, std::nullopt, std::nullopt, method);
        }

        std::cout << "Generated skills:\n";
        std::cout << "\tStrength: " << skills[Adndtk::Defs::skill::strength] << "\n";
        std::cout << "\tDexterity: " << skills[Adndtk::Defs::skill::dexterity] << "\n";
        std::cout << "\tConstitution: " << skills[Adndtk::Defs::skill::constitution] << "\n";
        std::cout << "\tIntelligence: " << skills[Adndtk::Defs::skill::intelligence] << "\n";
        std::cout << "\tWisdom: " << skills[Adndtk::Defs::skill::wisdom] << "\n";
        std::cout << "\tCharisma: " << skills[Adndtk::Defs::skill::charisma] << "\n";

        auto yesNo = prompt<std::string>("Accept data? [Y/n]");
        accept = (yesNo == "y" || yesNo == "Y");
    }
    while (!accept);

    std::cout << "Yous skills are "
                << skills[Adndtk::Defs::skill::strength] << " "
                << skills[Adndtk::Defs::skill::dexterity] << " "
                << skills[Adndtk::Defs::skill::constitution] << " "
                << skills[Adndtk::Defs::skill::intelligence] << " "
                << skills[Adndtk::Defs::skill::wisdom] << " "
                << skills[Adndtk::Defs::skill::charisma] << "\n";

    accept = false;
    auto races = Adndtk::CharacterGenerator::available_races(skills);
    Adndtk::Tables::race selectedRace{};
    do
    {
        std::cout << "Choose a race...\n";
        std::map<std::string, Adndtk::Tables::race> acronyms{};
        for (auto& r : races)
        {
            std::cout << "\t[" << r.acronym << "]:\t" << r.name << "\n";
            acronyms[r.acronym] = r;
        }

        auto acronym = prompt<std::string>("Choose one");
        accept = (acronyms.find(acronym) != acronyms.end());
        if (accept)
        {
            selectedRace = acronyms[acronym];
        }
    }
    while (!accept);

    std::cout << "Your choice is " << selectedRace.name << ".\n";

    accept = false;
    auto classes = Adndtk::CharacterGenerator::available_classes(skills, static_cast<Adndtk::Defs::race>(selectedRace.id));
    Adndtk::Tables::character_class selectedClass{};
    do
    {
        std::cout << "Choose a class...\n";
        std::map<std::string, Adndtk::Tables::character_class> acronyms{};
        for (auto& c : classes)
        {
            std::cout << "\t[" << c.acronym << "]:\t" << c.long_name << "\n";
            acronyms[c.acronym] = c;
        }

        auto acronym = prompt<std::string>("Choose one");
        accept = (acronyms.find(acronym) != acronyms.end());
        if (accept)
        {
            selectedClass = acronyms[acronym];
        }
    }
    while (!accept);

    std::cout << "Your choice is " << selectedClass.long_name << ".\n";

    auto clsId = static_cast<Adndtk::Defs::character_class>(selectedClass.id);
    auto raceId = static_cast<Adndtk::Defs::race>(selectedRace.id);
    if (Adndtk::Cyclopedia::get_instance().can_have_exceptional_strength(clsId, raceId, skills[Adndtk::Defs::skill::strength]))
    {
        Adndtk::Die d{Adndtk::Defs::die::d100};
        auto excValue = d.roll();
        skills[Adndtk::Defs::skill::strength].setExceptionalStrength(excValue);
        std::cout << "The character is entitoled to exceptional strength. Strength value changed to " << skills[Adndtk::Defs::skill::strength] << ".\n";
    }

    accept = false;
    auto alignments = Adndtk::CharacterGenerator::available_moral_alignments(clsId);
    Adndtk::Tables::moral_alignment selectedAlignment{};
    do
    {
        std::cout << "Choose your moral alignment...\n";
        std::map<std::string, Adndtk::Tables::moral_alignment> acronyms{};
        for (auto& m : alignments)
        {
            std::cout << "\t[" << m.acronym << "]:\t" << m.name << "\n";
            acronyms[m.acronym] = m;
        }

        auto acronym = prompt<std::string>("Choose one");
        accept = (acronyms.find(acronym) != acronyms.end());
        if (accept)
        {
            selectedAlignment = acronyms[acronym];
        }
    }
    while (!accept);

    std::cout << "Your choice is " << selectedAlignment.name << ".\n";

    accept = false;
    auto sex = Adndtk::CharacterGenerator::available_sex();
    Adndtk::Tables::sex selectedSex{};
    do
    {
        std::cout << "Choose your sex...\n";
        std::map<char, Adndtk::Tables::sex> acronyms{};
        for (auto& s : sex)
        {
            std::cout << "\t[" << s.acronym << "]:\t" << s.name << "\n";
            acronyms[s.acronym] = s;
        }

        auto acronym = prompt<std::string>("Choose one");
        accept = (acronyms.find(acronym.at(0)) != acronyms.end());
        if (accept)
        {
            selectedSex = acronyms[acronym.at(0)];
        }
    }
    while (!accept);

    std::cout << "Your choice is " << selectedSex.name << ".\n";

    accept = false;
    auto deities = Adndtk::CharacterGenerator::available_deities();
    Adndtk::Tables::deity selectedDeity{};
    do
    {
        std::cout << "Choose your deity...\n";
        std::map<int, Adndtk::Tables::deity> acronyms{};
        for (auto& d : deities)
        {
            std::cout << "\t[" << d.id << "]:\t" << d.name << "\n";
            acronyms[d.id] = d;
        }

        auto acronym = prompt<int>("Choose one");
        accept = (acronyms.find(acronym) != acronyms.end());
        if (accept)
        {
            selectedDeity = acronyms[acronym];
        }
    }
    while (!accept);

    std::cout << "Your choice is " << selectedDeity.name << ".\n";

    std::cout << "Choose a name for your character\n";
    auto charName = prompt<std::string>("Character name");
    std::cout << charName << " is being generated...\n";

    // Adndtk::Character chr{charName,
    //         static_cast<Adndtk::Defs::character_class>(selectedClass.id),
    //         static_cast<Adndtk::Defs::race>(selectedRace.id),
    //         static_cast<Adndtk::Defs::moral_alignment>(selectedAlignment.id),
    //         static_cast<Adndtk::Defs::sex>(selectedSex.id),
    //         static_cast<Adndtk::Defs::deity>(selectedDeity.id),
    // };

    // std::cout << "Character summary:\n";

    // std::cout << "Strength: " << chr.strength() << "\n";
    // auto strStats = Adndtk::SkillStats::get_instance().get_strength_stats(chr.strength());
    // std::cout << "\tHit Probability:\t" << strStats.hit_modifier << "\n";
    // std::cout << "\tDamage Adjustment:\t" << strStats.damage_adjustment << "\n";
    // std::cout << "\tWeight Allowance:\t" << strStats.weight_allowance << "\n";
    // std::cout << "\tMaximum Press:\t" << strStats.maximum_press << "\n";
    // std::cout << "\tOpen Doors:\t" << strStats.open_doors << " (" << strStats.open_doors_special.value_or(0) << ")\n";
    // std::cout << "\tBend Bars/Lift Gates:\t" << strStats.bend_bars_lift_gates << "\n";
    // std::cout << "\tNotes:\t" << strStats.note.value_or("") << "\n";

    // std::cout << "Dexterity: " << chr.dexterity() << "\n";
    // auto dexStats = Adndtk::SkillStats::get_instance().get_dexterity_stats(chr.dexterity());
    // std::cout << "\tReaction Adjustment:\t" << dexStats.reaction_adjustment << "\n";
    // std::cout << "\tMissile Attack Adjustment:\t" << dexStats.missile_attack_adjustment << "\n";
    // std::cout << "\tDefensive Adjustment:\t" << dexStats.defensive_adjustment << "\n";

    // std::cout << "Constitution: " << chr.constitution() << "\n";
    // auto conStats = Adndtk::SkillStats::get_instance().get_constitution_stats(chr.constitution());
    // std::cout << "\tHit Point Adjustment:\t" << conStats.hit_point_adjustment << " (" << conStats.hit_point_adjustment_warriors.value_or(0) << ")\n";
    // std::cout << "\tSystem Shock:\t" << conStats.system_shock << "\n";
    // std::cout << "\tResurrection Survival:\t" << conStats.resurrection_survival << "\n";
    // std::cout << "\tPoison Save:\t" << conStats.poison_save << "\n";
    // std::cout << "\tRegeneration:\t" << conStats.regeneration_points.value_or(0) << "/" << conStats.regeneration_turns.value_or(0) << "\n";
    
    // std::cout << "Intelligence: " << chr.intelligence() << "\n";
    // auto intStats = Adndtk::SkillStats::get_instance().get_intelligence_stats(chr.intelligence());
    // std::cout << "\tNo. of Languages:\t" << intStats.number_of_languages << "\n";
    // std::cout << "\tSpell Level:\t" << intStats.spell_level.value_or(0) << "\n";
    // std::cout << "\tChance to Learn Spell:\t" << intStats.chance_to_learn_spell.value_or(0) << "\n";
    // std::cout << "\tMaximum Number of Spells per Level:\t" << intStats.max_number_of_spells_per_level.value_or(0) << "\n";
    // //std::cout << "\tSpell Immunity:\t" << intStats.s << "\n";    
    
    // std::cout << "Wisdom: " << chr.wisdom() << "\n";
    // auto wisStats = Adndtk::SkillStats::get_instance().get_wisdom_stats(chr.wisdom());
    // std::cout << "\tMagical Defense Adjustment:\t" << wisStats.magical_defence_adjustment << "\n";
    // std::cout << "\tBonus Spells:\t" << wisStats.bonus_spell_level_1.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_2.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_3.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_4.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_5.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_6.value_or(0)
    //                                 << "/" << wisStats.bonus_spell_level_7.value_or(0)
    //                                 << "\n";
    // std::cout << "\tChance of Spell Failure:\t" << wisStats.chance_of_spell_failure << "\n";
    // std::cout << "\tSpell Immunity:\t" << wisStats.spell_immunity.value_or(0) << "\n";
    
    // std::cout << "Charisma: " << chr.charisma() << "\n";
    // auto chaStats = Adndtk::SkillStats::get_instance().get_charisma_stats(chr.charisma());
    // std::cout << "\tMaximum Number of Henchmen:\t" << chaStats.maximum_number_of_henchmen << "\n";
    // std::cout << "\tLoyalty Base:\t" << chaStats.loyalty_base << "\n";
    // std::cout << "\tReaction Adjustment:\t" << chaStats.reaction_adjustment.value_or(0) << "\n";
}