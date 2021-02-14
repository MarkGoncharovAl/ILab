#include "Errors.hpp"
[[noreturn]] void MLib::print_error(std::string info, std::string FILE, std::size_t LINE)
{
    std::cout << Color::Bold_Red << std::endl
              << info << std::endl 
              << "Mistake was found in file " << FILE
              << "\nMistake was found in line " << LINE 
              << std::endl << Color::Reset;
    exit(EXIT_FAILURE);
}
void MLib::print_warning(std::string info, std::string FILE, std::size_t LINE)
{
    std::cout << Color::Bold_Red << std::endl
              << info << std::endl 
              << "Warning was found in file " << FILE
              << "\nWarning was found in line " << LINE 
              << std::endl << Color::Reset;
}
