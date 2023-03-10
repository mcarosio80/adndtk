#include <iostream>
#include <adndtk.h>

void demo_cyclopedia();

int main(int argc, char **argv)
{
    std::cout << Adndtk::Metadata::library_name
                << " " << Adndtk::Metadata::version
                << " by " << Adndtk::Metadata::author << "\n";
    std::cout << "Last library commit: " << Adndtk::Metadata::last_commit_date << "(" << Adndtk::Metadata::last_commit_hash <<")" << "\n";
    
    std::cout << "\nDESCRIPTION:\n" << Adndtk::Metadata::desc << "\n\n";

    demo_cyclopedia();

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
}