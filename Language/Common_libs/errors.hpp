#pragma once
#include <iostream>

namespace help {

	[[noreturn]] void print_error(std::string info, std::string FILE, std::size_t LINE);
	void print_warning(std::string info, std::string FILE, std::size_t LINE);

	template <class T>
	inline void debug_print(const T& elem1, const T& elem2 = T{})
	{
		std::cout << "Debagging is starting!\n" << elem1 << "\t" << elem2 << "\nDebagging is ending";
	}
}

#define ERROR(str)   help::print_error  (str, __FILE__, __LINE__)
#define WARNING(str) help::print_warning(str, __FILE__, __LINE__)

#define DEBUG1(elem1) help::debug_print(elem1)
#define DEBUG2(elem1, elem2) help::debug_print(elem1, elem2)
