#include <iostream>
#include <string>
#include <sstream>
#include <optional>
#include <set>
#include <map>
#include <iomanip>

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
std::optional<Adndtk::Tables::deity> choose_deity(const Adndtk::Defs::character_class& clsId, const Adndtk::Defs::moral_alignment& alignId);

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

std::optional<Adndtk::Tables::deity> choose_deity(const Adndtk::Defs::character_class& clsId, const Adndtk::Defs::moral_alignment& alignId)
{
    auto yesNo = CliTools::prompt<std::string>("Choose your faith? [Y/n]");
    bool accept = (yesNo == "y" || yesNo == "Y");

    if (!accept)
    {
        return std::nullopt;
    }

    auto cults = Adndtk::Tables::cult::to_map<Adndtk::Defs::cult>("id");
    auto ranks = Adndtk::Tables::deity_rank::to_map<Adndtk::Defs::deity_rank>("id");
    
    auto formatDeityMenu = [&](const Adndtk::Tables::deity& r, std::map<int, Adndtk::Tables::deity>& menu) -> void
    {
        auto cultId = static_cast<Adndtk::Defs::cult>(r.cult);
        auto rankId = static_cast<Adndtk::Defs::deity_rank>(r.rank);
        std::cout << "\t[" << r.id << "]:\t" << r.name << " (status: " << ranks[rankId].description << ", cult: " << cults[cultId].name << ")\n";
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
    std::optional<Adndtk::Defs::deity> optDeityId{std::nullopt};
    if (selectedDeity.has_value())
    {
        optDeityId = static_cast<Adndtk::Defs::deity>(selectedDeity.value().id);
        std::cout << "Your choice is " << selectedDeity.value().name << ".\n";
    }
    else
    {
        std::cout << "Your chose no faith.\n";
    }

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
            optDeityId,
    };

    chr.change_skill(skills[Adndtk::Defs::skill::strength]);
    chr.change_skill(skills[Adndtk::Defs::skill::dexterity]);
    chr.change_skill(skills[Adndtk::Defs::skill::constitution]);
    chr.change_skill(skills[Adndtk::Defs::skill::intelligence]);
    chr.change_skill(skills[Adndtk::Defs::skill::wisdom]);
    chr.change_skill(skills[Adndtk::Defs::skill::charisma]);

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
    std::cout << "\t" << std::left << std::setw(40) << "Hit Probability:" << strStats.hit_modifier << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Damage Adjustment:" << strStats.damage_adjustment << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Weight Allowance:" << strStats.weight_allowance << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Maximum Press:" << strStats.maximum_press << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Open Doors:" << strStats.open_doors << " (" << strStats.open_doors_special.value_or(0) << ")\n";
    std::cout << "\t" << std::left << std::setw(40) << "Bend Bars/Lift Gates:" << strStats.bend_bars_lift_gates << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Notes:" << strStats.note.value_or("") << "\n";

    std::cout << "Dexterity: " << chr.dexterity() << "\n";
    auto dexStats = Adndtk::SkillStats::get_instance().get_dexterity_stats(chr.dexterity());
    std::cout << "\t" << std::left << std::setw(40) << "Reaction Adjustment:" << dexStats.reaction_adjustment << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Missile Attack Adjustment:" << dexStats.missile_attack_adjustment << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Defensive Adjustment:" << dexStats.defensive_adjustment << "\n";

    std::cout << "Constitution: " << chr.constitution() << "\n";
    auto conStats = Adndtk::SkillStats::get_instance().get_constitution_stats(chr.constitution());
    std::cout << "\t" << std::left << std::setw(40) << "Hit Point Adjustment:" << conStats.hit_point_adjustment << " (" << conStats.hit_point_adjustment_warriors.value_or(0) << ")\n";
    std::cout << "\t" << std::left << std::setw(40) << "System Shock:" << conStats.system_shock << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Resurrection Survival:" << conStats.resurrection_survival << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Poison Save:" << conStats.poison_save << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Regeneration:" << conStats.regeneration_points.value_or(0) << "/" << conStats.regeneration_turns.value_or(0) << "\n";
    
    std::cout << "Intelligence: " << chr.intelligence() << "\n";
    auto intStats = Adndtk::SkillStats::get_instance().get_intelligence_stats(chr.intelligence());
    std::cout << "\t" << std::left << std::setw(40) << "No. of Languages:" << intStats.number_of_languages << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Spell Level:" << intStats.spell_level.value_or(0) << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Chance to Learn Spell:" << intStats.chance_to_learn_spell.value_or(0) << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Maximum Number of Spells per Level:" << intStats.max_number_of_spells_per_level.value_or(0) << "\n";
    //std::cout << "\t" << std::left << std::setw(40) << "Spell Immunity:" << intStats.s << "\n";    
    
    std::cout << "Wisdom: " << chr.wisdom() << "\n";
    auto wisStats = Adndtk::SkillStats::get_instance().get_wisdom_stats(chr.wisdom());
    std::cout << "\t" << std::left << std::setw(40) << "Magical Defense Adjustment:" << wisStats.magical_defence_adjustment << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Bonus Spells:" << wisStats.bonus_spell_level_1.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_2.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_3.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_4.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_5.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_6.value_or(0)
                                    << "/" << wisStats.bonus_spell_level_7.value_or(0)
                                    << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Chance of Spell Failure:" << wisStats.chance_of_spell_failure << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Spell Immunity:" << wisStats.spell_immunity.value_or(0) << "\n";
    
    std::cout << "Charisma: " << chr.charisma() << "\n";
    auto chaStats = Adndtk::SkillStats::get_instance().get_charisma_stats(chr.charisma());
    std::cout << "\t" << std::left << std::setw(40) << "Maximum Number of Henchmen:" << chaStats.maximum_number_of_henchmen << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Loyalty Base:" << chaStats.loyalty_base << "\n";
    std::cout << "\t" << std::left << std::setw(40) << "Reaction Adjustment:" << chaStats.reaction_adjustment.value_or(0) << "\n";

    std::cout << "\nCombat:\n";
    auto xps = chr.experience();
    std::cout << "\t" << std::left << std::setw(10) << "Experience:\n";
    auto classes = Adndtk::Tables::character_class::to_map<Adndtk::Defs::character_class>("id");
    for (auto& xp : xps)
    {
        std::cout
                << "\t\t" << std::left << std::setw(8) << classes[xp.first].acronym
                << "level " << xps.level(xp.first) << " (" << xps.xp(xp.first) << " xp)\n";
    }
    std::cout << "\n";
    std::cout << "\t" << std::left << std::setw(10) << "HP: " << chr.total_hp() << "\n";
    //std::cout << "\t" << std::left << std::setw(10) << "THAC0: " << chr.thaco() << "\n";

    std::cout << "\nSaving Throws:\n";
    auto savingThrows = Adndtk::Tables::saving_throw::fetch_all();
    for (auto& st : savingThrows)
    {
        auto savId = static_cast<Adndtk::Defs::saving_throw>(st.id);
        std::cout << "\t" << std::left << std::setw(20) << st.description << chr.save_score(savId) << "\n";
    }

    std::cout << "\nRacial traits:\n";
    std::cout << "\t" << std::left << std::setw(20) << "Age: " << chr.age() << "\n";
    std::cout << "\t" << std::left << std::setw(20) << "Weight: " << chr.weight() << "\n";
    std::cout << "\t" << std::left << std::setw(20) << "Encumbrance: " << chr.equipment_weight() << "\n";
    std::cout << "\t" << std::left << std::setw(20) << "Total weight: " << chr.total_weight() << "\n";
    std::cout << "\t" << std::left << std::setw(20) << "Height: " << chr.height() << "\n";
    std::cout << "\t" << std::left << std::setw(20) << "Movement factor: " << chr.movement_factor() << "\n";

    std::cout << "\nMoney:\n";
    auto coins = Adndtk::Tables::coin::fetch_all();
    for (auto& c : coins)
    {
        auto type = static_cast<Adndtk::Defs::coin>(c.id);
        std::cout << "\t" << std::left << std::setw(4) << c.acronym << std::right << std::setw(3) << chr.money()[type] << "\n";
    }
}