#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <set>
#include <map>

#include <adndtk.h>
#include <cli_options.h>
#include <cli_validator.h>
#include <cli_menu.h>
#include <character_generator.h>

Adndtk::Character generate_character(const Adndtk::SkillGenerationMethod& method);
//Adndtk::Character generate_character(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills);
void print_summary(const Adndtk::Character& chr);

std::map<Adndtk::Defs::skill, Adndtk::SkillValue> generate_skills(const Adndtk::SkillGenerationMethod& method);
Adndtk::Tables::race choose_race(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills);
Adndtk::Tables::character_class choose_class(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills, const Adndtk::Defs::race& raceId);
Adndtk::Tables::moral_alignment choose_moral_alignment(const Adndtk::Defs::character_class& clsId);
Adndtk::Tables::sex choose_sex();
Adndtk::Tables::deity choose_deity(const Adndtk::Defs::character_class& clsId, const Adndtk::Defs::moral_alignment& alignId);

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
                auto chr = generate_character(genMethod.value());
                print_summary(chr);
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
    else if (cliOpts.has_option(CliTools::Option::skills)) {
        std::cout << "Entering skills-based character generation..." << "\n\n";
        auto argVal = cliOpts.get_option<std::string>(CliTools::Option::skills);
        if (argVal.has_value())
        {
            auto skills = validator.validate_skills_list(argVal.value());
            if (skills.has_value())
            {
                // auto chr = generate_character(genMethod.value());
                // print_summary(chr);
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

std::map<Adndtk::Defs::skill, Adndtk::SkillValue> generate_skills(const Adndtk::SkillGenerationMethod& method)
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

        auto yesNo = CliTools::prompt<std::string>("Accept data? [Y/n]");
        accept = (yesNo == "y" || yesNo == "Y");
    }
    while (!accept);

    return skills;
}

Adndtk::Tables::race choose_race(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills)
{
    auto formatRaceMenu = [](const Adndtk::Tables::race& r, std::map<std::string, Adndtk::Tables::race>& menu) -> void
    {
        std::cout << "\t[" << r.acronym << "]:\t" << r.name << "\n";
        menu[r.acronym] = r;
    };
    auto races = Adndtk::CharacterGenerator::available_races(skills);
    CliTools::CliMenu<Adndtk::Tables::race, std::string> raceMenu{"Choose a race", races};
    raceMenu.line_formatter() = formatRaceMenu;
    auto selectedRace = raceMenu();

    return selectedRace;
}

Adndtk::Tables::character_class choose_class(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills, const Adndtk::Defs::race& raceId)
{
    auto formatClassMenu = [](const Adndtk::Tables::character_class& r, std::map<std::string, Adndtk::Tables::character_class>& menu) -> void
    {
        std::cout << "\t[" << r.acronym << "]:\t" << r.long_name << "\n";
        menu[r.acronym] = r;
    };
    auto classes = Adndtk::CharacterGenerator::available_classes(skills, raceId);
    CliTools::CliMenu<Adndtk::Tables::character_class, std::string> classMenu{"Choose a class", classes};
    classMenu.line_formatter() = formatClassMenu;
    auto selectedClass = classMenu();

    return selectedClass;
}

Adndtk::Tables::moral_alignment choose_moral_alignment(const Adndtk::Defs::character_class& clsId)
{
    auto formatAlignMenu = [](const Adndtk::Tables::moral_alignment& r, std::map<std::string, Adndtk::Tables::moral_alignment>& menu) -> void
    {
        std::cout << "\t[" << r.acronym << "]:\t" << r.name << "\n";
        menu[r.acronym] = r;
    };
    auto alignments = Adndtk::CharacterGenerator::available_moral_alignments(clsId);
    CliTools::CliMenu<Adndtk::Tables::moral_alignment, std::string> alignMenu{"Choose your moral alignment", alignments};
    alignMenu.line_formatter() = formatAlignMenu;
    auto selectedAlignment = alignMenu();

    return selectedAlignment;
}

Adndtk::Tables::sex choose_sex()
{
    auto formatSexMenu = [](const Adndtk::Tables::sex& r, std::map<char, Adndtk::Tables::sex>& menu) -> void
    {
        std::cout << "\t[" << r.acronym << "]:\t" << r.name << "\n";
        menu[r.acronym] = r;
    };
    auto sex = Adndtk::CharacterGenerator::available_sex();
    CliTools::CliMenu<Adndtk::Tables::sex, char> sexMenu{"Choose one", sex};
    sexMenu.line_formatter() = formatSexMenu;
    auto selectedSex = sexMenu();

    return selectedSex;
}

Adndtk::Tables::deity choose_deity(const Adndtk::Defs::character_class& clsId, const Adndtk::Defs::moral_alignment& alignId)
{
    auto formatDeityMenu = [](const Adndtk::Tables::deity& r, std::map<int, Adndtk::Tables::deity>& menu) -> void
    {
        std::cout << "\t[" << r.id << "]:\t" << r.name << "\n";
        menu[r.id] = r;
    };
    auto deities = Adndtk::CharacterGenerator::available_deities(alignId);
    CliTools::CliMenu<Adndtk::Tables::deity, int> deityMenu{"Choose one", deities};
    deityMenu.line_formatter() = formatDeityMenu;
    auto selectedDeity = deityMenu();

    return selectedDeity;
}

Adndtk::Character generate_character(const Adndtk::SkillGenerationMethod& method)
{
    // Roll Ability Scores
    auto skills = generate_skills(method);
    std::cout << "Yous skills are "
                << skills[Adndtk::Defs::skill::strength] << " "
                << skills[Adndtk::Defs::skill::dexterity] << " "
                << skills[Adndtk::Defs::skill::constitution] << " "
                << skills[Adndtk::Defs::skill::intelligence] << " "
                << skills[Adndtk::Defs::skill::wisdom] << " "
                << skills[Adndtk::Defs::skill::charisma] << "\n";

    // Choose a Race
    auto selectedRace = choose_race(skills);
    auto raceId = static_cast<Adndtk::Defs::race>(selectedRace.id);
    std::cout << "Your race is " << selectedRace.name << ".\n";

    // Select a Class
    auto selectedClass = choose_class(skills, static_cast<Adndtk::Defs::race>(selectedRace.id));
    auto clsId = static_cast<Adndtk::Defs::character_class>(selectedClass.id);
    std::cout << "Your class is " << selectedClass.long_name << ".\n";

    // Determine if entitoled to exceptional strength
    if (Adndtk::Cyclopedia::get_instance().can_have_exceptional_strength(clsId, raceId, skills[Adndtk::Defs::skill::strength]))
    {
        Adndtk::Die d{Adndtk::Defs::die::d100};
        auto excValue = d.roll();
        skills[Adndtk::Defs::skill::strength].setExceptionalStrength(excValue);
        std::cout << "The character is entitoled to exceptional strength. Strength value changed to " << skills[Adndtk::Defs::skill::strength] << ".\n";
    }

    // Choose an Alignment
    auto selectedAlignment = choose_moral_alignment(clsId);
    auto alignId = static_cast<Adndtk::Defs::moral_alignment>(selectedAlignment.id);
    std::cout << "Your choice is " << selectedAlignment.name << ".\n";

    // Choose a Sex
    auto selectedSex = choose_sex();
    std::cout << "Your choice is " << selectedSex.name << ".\n";

    // Select a faith
    auto selectedDeity = choose_deity(clsId, alignId);
    std::cout << "Your choice is " << selectedDeity.name << ".\n";

    std::cout << "Choose a name for your character\n";
    auto charName = CliTools::prompt<std::string>("Character name");
    std::cout << charName << " is being generated...\n";

    // Select Proficiencies
    // Equip Your Character

    Adndtk::Character chr{charName,
            static_cast<Adndtk::Defs::character_class>(selectedClass.id),
            static_cast<Adndtk::Defs::race>(selectedRace.id),
            static_cast<Adndtk::Defs::moral_alignment>(selectedAlignment.id),
            static_cast<Adndtk::Defs::sex>(selectedSex.id), 
            static_cast<Adndtk::Defs::deity>(selectedDeity.id),
    };
    return chr;
}

// Adndtk::Character generate_character(const std::map<Adndtk::Defs::skill, Adndtk::SkillValue>& skills)
// {
//     return {};
// }

void print_summary(const Adndtk::Character& chr)
{
    std::cout << "Character summary:\n";

    std::cout << "Strength: " << chr.strength() << "\n";
    auto strStats = Adndtk::SkillStats::get_instance().get_strength_stats(chr.strength());
    std::cout << "\tHit Probability:\t" << strStats.hit_modifier << "\n";
    std::cout << "\tDamage Adjustment:\t" << strStats.damage_adjustment << "\n";
    std::cout << "\tWeight Allowance:\t" << strStats.weight_allowance << "\n";
    std::cout << "\tMaximum Press:\t" << strStats.maximum_press << "\n";
    std::cout << "\tOpen Doors:\t" << strStats.open_doors << " (" << strStats.open_doors_special.value_or(0) << ")\n";
    std::cout << "\tBend Bars/Lift Gates:\t" << strStats.bend_bars_lift_gates << "\n";
    std::cout << "\tNotes:\t" << strStats.note.value_or("") << "\n";

    std::cout << "Dexterity: " << chr.dexterity() << "\n";
    auto dexStats = Adndtk::SkillStats::get_instance().get_dexterity_stats(chr.dexterity());
    std::cout << "\tReaction Adjustment:\t" << dexStats.reaction_adjustment << "\n";
    std::cout << "\tMissile Attack Adjustment:\t" << dexStats.missile_attack_adjustment << "\n";
    std::cout << "\tDefensive Adjustment:\t" << dexStats.defensive_adjustment << "\n";

    std::cout << "Constitution: " << chr.constitution() << "\n";
    auto conStats = Adndtk::SkillStats::get_instance().get_constitution_stats(chr.constitution());
    std::cout << "\tHit Point Adjustment:\t" << conStats.hit_point_adjustment << " (" << conStats.hit_point_adjustment_warriors.value_or(0) << ")\n";
    std::cout << "\tSystem Shock:\t" << conStats.system_shock << "\n";
    std::cout << "\tResurrection Survival:\t" << conStats.resurrection_survival << "\n";
    std::cout << "\tPoison Save:\t" << conStats.poison_save << "\n";
    std::cout << "\tRegeneration:\t" << conStats.regeneration_points.value_or(0) << "/" << conStats.regeneration_turns.value_or(0) << "\n";
    
    std::cout << "Intelligence: " << chr.intelligence() << "\n";
    auto intStats = Adndtk::SkillStats::get_instance().get_intelligence_stats(chr.intelligence());
    std::cout << "\tNo. of Languages:\t" << intStats.number_of_languages << "\n";
    std::cout << "\tSpell Level:\t" << intStats.spell_level.value_or(0) << "\n";
    std::cout << "\tChance to Learn Spell:\t" << intStats.chance_to_learn_spell.value_or(0) << "\n";
    std::cout << "\tMaximum Number of Spells per Level:\t" << intStats.max_number_of_spells_per_level.value_or(0) << "\n";
    //std::cout << "\tSpell Immunity:\t" << intStats.s << "\n";    
    
    std::cout << "Wisdom: " << chr.wisdom() << "\n";
    auto wisStats = Adndtk::SkillStats::get_instance().get_wisdom_stats(chr.wisdom());
    std::cout << "\tMagical Defense Adjustment:\t" << wisStats.magical_defence_adjustment << "\n";
    std::cout << "\tBonus Spells:\t" << wisStats.bonus_spell_level_1.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_2.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_3.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_4.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_5.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_6.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_7.value_or(0)
                                    << "\n";
    std::cout << "\tChance of Spell Failure:\t" << wisStats.chance_of_spell_failure << "\n";
    std::cout << "\tSpell Immunity:\t" << wisStats.spell_immunity.value_or(0) << "\n";
    
    std::cout << "Charisma: " << chr.charisma() << "\n";
    auto chaStats = Adndtk::SkillStats::get_instance().get_charisma_stats(chr.charisma());
    std::cout << "\tMaximum Number of Henchmen:\t" << chaStats.maximum_number_of_henchmen << "\n";
    std::cout << "\tLoyalty Base:\t" << chaStats.loyalty_base << "\n";
    std::cout << "\tReaction Adjustment:\t" << chaStats.reaction_adjustment.value_or(0) << "\n";
}