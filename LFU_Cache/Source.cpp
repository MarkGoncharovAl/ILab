#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include "Cache.h"


class page_t
{
public:
	explicit page_t(int init_id) noexcept :
		id_{ init_id }
	{}
	int get_id() const noexcept { return id_; }

private:
	int id_;
};

class Time
{
public:
	Time() noexcept :
		start{ std::chrono::high_resolution_clock::now() }
	{}
	std::chrono::milliseconds 
		GetAndResetTime() noexcept
	{
		auto end = std::chrono::high_resolution_clock::now();
		start = end;
		return std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> start;
};

class Random
{
public:

	Random(int init_start = std::mt19937::min(), int init_end = std::mt19937::max()) noexcept :
		dist_{init_start, init_end}
	{}

	int get() const noexcept { return dist_(gen_); }

private:
	static std::random_device       rd_;
	static std::mt19937             gen_;
	
	std::uniform_int_distribution<> dist_;
};

std::random_device Random::rd_;
std::mt19937       Random::gen_{ Random::rd_() };




void CountHitsCout();
void CountHits    (std::ifstream& inFile, std::ofstream& outFile);
void CountRandHits(std::ifstream& inFile, std::ofstream& outFile);



int main()
{
	std::ifstream InFileTests;
	std::ifstream InFileRand;
	std::ofstream OutFileTests;
	std::ofstream OutFileRand;

	InFileTests. open("Input_Tests.txt");
	OutFileTests.open("Output_Tests.txt");
	InFileRand.  open("InputRandomTests.txt");
	OutFileRand. open("OutputRandomTests.txt");

	if (InFileTests.fail() || OutFileTests.fail() || InFileRand.fail() || OutFileRand.fail()) {
		std::cout << "Can't reach files with tests!\n";
		std::abort();
	}

	//CountHitsCout();
	CountHits      (InFileTests, OutFileTests);
	CountRandHits  (InFileRand, OutFileRand);
}

void CountHitsCout()
{
	Time current_time;

	size_t size_cache = 0, hits = 0;
	int num_pages = 0;

	std::cin >> size_cache >> num_pages;
	cache_t<page_t> my_cash{ size_cache };

	for (int i = 0; i < num_pages; ++i) {

		int init_id = 0;
		std::cin >> init_id;
		page_t cur_page{ init_id };

		if (my_cash.lookup(&cur_page))
			hits++;
	}
	std::cout << "Final hits: " << hits << " and final time: " << current_time.GetAndResetTime().count() << "ms" << std::endl;
}
void CountHits(std::ifstream& inFile, std::ofstream& outFile)
{
	Time current_time;

	while (!inFile.eof()) {

		size_t size_cache = 0, hits = 0;
		int num_pages = 0;

		inFile >> size_cache >> num_pages;
		cache_t<page_t> my_cash{ size_cache };

		for (int i = 0; i < num_pages; ++i) {

			int init_id = 0;
			inFile >> init_id;
			page_t cur_page{ init_id };

			if (my_cash.lookup(&cur_page))
				hits++;

			my_cash.print_cash(outFile);
		}
		outFile << "Final hits: " << hits << " and final time: " << current_time.GetAndResetTime().count() << "ms" << std::endl;
	}
}
void CountRandHits(std::ifstream& inFile, std::ofstream& outFile)
{
	Time current_time;

	while (!inFile.eof()) {

		size_t size_cache = 0, hits = 0;
		int num_pages = 0, rage = 0;

		inFile >> size_cache >> num_pages >> rage;
		
		cache_t<page_t> my_cash{ size_cache };
		Random rand_num(0, rage);

		for (int i = 0; i < num_pages; ++i) {

			page_t cur_page{ rand_num.get() };

			if (my_cash.lookup(&cur_page))
				hits++;
		}
		outFile << "Final hits: " << hits << " and final time: " << current_time.GetAndResetTime().count() << "ms" << std::endl;
	}
}