#include <iostream>
#include "Libraries/base_data.hpp"
#include "Common_libs/time.hpp"
#include <vector>
#include <set>
#include <chrono>

void check_finding(const base_data<int> &data);
void check_counting(const base_data<int> &data);

int main()
{
	int size = 0;
	std::cin >> size;
	if (!std::cin.good())
		ERROR("Problems with reading from file!");

	base_data<int> set;

	for (int i = 0; i < size; ++i)
	{
		int num = 0;
		std::cin >> num;

		set.insert(num);
		if (!std::cin.good())
			ERROR("Problems with reading from file!");
	}

	check_finding(set);
	check_counting(set);
}

void check_finding(const base_data<int> &data)
{
	size_t size_test1 = 0;
	std::cin >> size_test1;
	if (!std::cin.good())
		ERROR("Problems with reading from file!");

	for (size_t i = 0; i < size_test1; ++i)
	{
		int num = 0;
		std::cin >> num;
		if (!std::cin.good())
			ERROR("Problems with reading from file!");

		std::cout << *(data.FindMin(num)) << " ";
	}
}
void check_counting(const base_data<int> &data)
{
	size_t size_test2 = 0;
	std::cin >> size_test2;
	if (!std::cin.good())
		ERROR("Problems with reading from file!");

	for (size_t i = 0; i < size_test2; ++i)
	{
		int num = 0;
		std::cin >> num;
		if (!std::cin.good())
			ERROR("Problems with reading from file!");

		std::cout << data.GetCountLess(num) << " ";
	}
}