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
    auto clsId = static_cast<Adndtk::Defs::character_class>(selectedClass.id);
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

    //auto charName = prompt<std::string>("Character name");
}