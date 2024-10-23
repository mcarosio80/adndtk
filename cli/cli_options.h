#ifndef ADNDTK_CLI_OPTIONS_H
#define ADNDTK_CLI_OPTIONS_H

#include <boost/program_options.hpp>

#include <fstream>
#include <sstream>
#include <optional>

namespace CliTools
{
    struct Option
    {
        static const char* help;
        static const char* generate;
        static const char* skills;
        static const char* char_class;
    };

    class CliOptions
    {
    public:
        CliOptions(int argc, char** argv);

        bool has_option(const char* opt) const
        {
            return _vm.count(opt) != 0;
        }
        const std::string get_help() const
        {
            std::stringstream ss{};
            ss << _desc;
            return ss.str();
        }
        friend std::ofstream& operator<<(std::ofstream& out, const CliOptions& opt)
        {
            out << opt.get_help();
            return out;
        }

        template<typename _Type>
        std::optional<_Type> get_option(const char* opt, const std::optional<_Type>& defaultValue = std::nullopt) const
        {
            if (has_option(opt))
            {
                return _vm[opt].as<_Type>();
            }
            if (defaultValue)
            {
                return defaultValue;
            }
            return std::nullopt;
        }

    private:
        boost::program_options::options_description _desc;
        boost::program_options::variables_map _vm;
    
        void add_options();
    };
} // namespace CliTools

#endif //ADNDTK_CLI_OPTIONS_H