#include <iostream>
#include "Base/Geometry.hpp"

int main ()
{
	size_t count = 0;
	std::cin >> count;
	if (std::cin.fail ())
		ERROR ("Can't read data properly!");

	std::vector<MLib::Triangle<double>> trs;
	trs.reserve (count);
	for (size_t i = 0; i < count; ++i)
	{
		std::array<double , 9> arr;
		for (double& elem : arr)
			std::cin >> elem;
		if (std::cin.fail ())
			ERROR ("Can't read data properly!");

		trs.push_back (std::move (MLib::Triangle { arr }));

		bool is_found = false;
		for (size_t j = 0; j < i; ++j)
		{
			if (MLib::Intersect(trs[i], trs[j]))
				is_found = true;
		}

		if (is_found)
			std::cout << i << std::endl;
	}
}