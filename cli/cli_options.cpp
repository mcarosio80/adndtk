#include <cli_options.h>

namespace CliTools
{
const char* Option::help = "help";
const char* Option::generate = "generate";
const char* Option::skills = "skills";
const char* Option::char_class = "class";


CliOptions::CliOptions(int argc, char** argv)
    : _desc{"Allowed options"}, _vm{}
{
    add_options();

    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, _desc), _vm);
    boost::program_options::notify(_vm);  
}

void CliOptions::add_options()
{
    _desc.add_options()
        (Option::help, "Adndtk CLI tool online help")
        ("generate,g",
            boost::program_options::value<std::string>(),
            "generate a new character (automatic mode), the tool queries for choices or missing data. "
            "Argument can be any of the following:\n"
            "[standard] - 3d6 per each skill\n"
            "[best4] - 4d6, the least of them is discarded\n"
            "[best-twice] - 3d6 twice, the best is chosen\n"
        )
        ("skills,s",
            boost::program_options::value<std::string>(),
            "generate a new character (skills mode), the tool queries for choices or missing data. "
            "Argument is a comma-separated list representing the six skill values in standard order "
            "(strength, dexterity, constitution, intelligence, wisdom, charisma)\n"
        )
        ("class,c",
            boost::program_options::value<std::string>(),
            "generate a new character (class mode), the tool queries for choices or missing data. "
            "Argument is a character class name or acronym "
            "(i.e. full class names like 'Paladin', 'Ranger', 'Cleric' or acronyms like 'F/M' for Fighter/Mage)\n"
        )
        ;
}

} // namespace AdndtkCli