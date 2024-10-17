#ifndef ADNDTK_CLI_MENU_H
#define ADNDTK_CLI_MENU_H

#include <vector>
#include <map>
#include <functional>
#include <string>

namespace CliTools
{
    template<typename _RetType = std::string>
    static _RetType prompt(const std::string& prompt)
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

    template<typename _TableName, typename _MenuKeyType>
    class CliMenu
    {
    public:
        CliMenu(const std::string& caption, const std::vector<_TableName>& data)
            : _menuCaption{caption}, _menuData{data},
            _menuLineFormatter{[](const _TableName& menuValue, std::map<_MenuKeyType, _TableName>& menu)->void{}}
        {
        }

        auto& line_formatter()
        {
            return _menuLineFormatter;
        }

        _TableName operator()()
        {
            bool accept = false;
            _TableName selected{};
            do
            {
                std::cout << _menuCaption << "...\n";
                std::map<_MenuKeyType, _TableName> menu{};
                for (auto& d : _menuData)
                {
                    _menuLineFormatter(d, menu);
                }

                auto choice = prompt<_MenuKeyType>("Choose one");
                accept = (menu.find(choice) != menu.end());
                if (accept)
                {
                    selected = menu[choice];
                }
            }
            while (!accept);

            return selected;
        }

    private:
        std::string                 _menuCaption;
        std::vector<_TableName>     _menuData;
        std::function<void(const _TableName& menuValue, std::map<_MenuKeyType, _TableName>& menu)>
                                    _menuLineFormatter;
    };
} // namespace CliTools

#endif //ADNDTK_CLI_MENU_H