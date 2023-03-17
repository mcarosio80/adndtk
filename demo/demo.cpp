#include <iostream>
#include <adndtk.h>

void demo_cyclopedia();
void demo_dice();

int main(int argc, char **argv)
{
    std::cout << Adndtk::Metadata::library_name
                << " " << Adndtk::Metadata::version
                << " by " << Adndtk::Metadata::author << "\n";
    std::cout << "Last library commit: " << Adndtk::Metadata::last_commit_date << "(" << Adndtk::Metadata::last_commit_hash <<")" << "\n";
    
    std::cout << "\nDESCRIPTION:\n" << Adndtk::Metadata::desc << "\n\n";

    demo_cyclopedia();
    demo_dice();

    return 0;
}

void demo_cyclopedia()
{
    auto onResult = [] (int row, int col, const char *cellType, const unsigned char *cellValue) -> void
    {
        if (Adndtk::Settings::query_type == Adndtk::QueryType::json)
        {
            std::cout << "Result[" << row << ", " << col << "] = " << cellValue << "\n";
        }
        else
        {
            std::cout << "Result[" << row << ", " << col << "] = " << cellType << ": " << cellValue << "\n";
        }
    };

    Adndtk::Cyclopedia::get_instance().exec_prepared_statement<int>(
        Adndtk::Cyclopedia::Query::select_coin,
        onResult,
        static_cast<int>(Adndtk::Defs::coin::gold_piece)
    );

    Adndtk::Defs::character_class_type type;
    auto onExecResult = [&type] (int row, int col, const char *columnName, const unsigned char *cellValue) -> void
    {
        std::string jsonValue{reinterpret_cast<const char*>(cellValue)};
    };
    Adndtk::Cyclopedia::get_instance().exec("select class_type_id from character_class where id = 9", onExecResult);
}

void demo_dice()
{
    Adndtk::Die d{Adndtk::Defs::die::d8};
    int numRoll = 3;

    for (int i=0; i<6; ++i)
    {
        int result = d.roll(numRoll);
        std::cout << "Result" << "[" << i+1 << "] of " << numRoll << d << ": " << result << "\n";
    }

    std::string expr{"4d4+1"};
    for (int i=0; i<6; ++i)
    {
        int result = Adndtk::Die::roll(expr.c_str());
        std::cout << "Result[" << i+1 << "] of " << expr << ": " << result << "\n";
    }

    Adndtk::Die d6;
    for (int i=0; i<6; ++i)
    {
        std::cout << "Result" << "[" << i+1 << "] of 1" << d6 << ": " << static_cast<int>(d6) << "\n";
    }

    std::string malformedExpr{"-3d4-"};
    try
    {
        int res = Adndtk::Die::roll(malformedExpr.c_str());
        std::cout << "Result of " << malformedExpr << ": " << res << "\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    std::string expr2{"3d5"};
    int res = Adndtk::Die::roll(expr2.c_str());
    std::cout << "Result of " << expr2 << ": " << res << "\n";

    std::string expr3{"4  -  10"};
    int res3 = Adndtk::Die::roll(expr3.c_str());
    std::cout << "Result of " << expr3 << ": " << res3 << "\n";
}