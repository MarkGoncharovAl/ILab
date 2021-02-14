#include "errors.hpp"
[[noreturn]] void help::print_error(std::string info, std::string FILE, std::size_t LINE)
{
	std::cout << std::endl
			  << info << std::endl;
	std::cout << "Mistake was found in file " << FILE;
	std::cout << "\nMistake was found in line " << LINE << std::endl;
	exit(EXIT_FAILURE);
}
void help::print_warning(std::string info, std::string FILE, std::size_t LINE)
{
	std::cout << std::endl
			  << info << std::endl;
	std::cout << "Warning was found in file " << FILE;
	std::cout << "\nWarning was found in line " << LINE << std::endl;
}
