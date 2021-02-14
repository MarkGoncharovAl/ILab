#include <iostream>
#include <set>
#include <fstream>
#include "Triangle.h"

#define double long double

/*
96.6944 28.8409 30.3789
95.9876 28.8786 30.6846
96.0249 28.8783 30.155
96.2479 29.0639 30.2841
96.1473 29.4089 30.0288
96.2053 29.3381 29.8109

33.3363 91.2551 67.8482
33.2543 90.9706 67.1548
33.6565 90.6161 67.7456
33.306  91.067  67.2561
33.9626 91.3808 66.8995
33.5757 90.5551 67.4595
*/

namespace geometry {
	////////////////////////////////////////////////////////////////////////////////
	template <class T1, class T2, class T3>
	struct Triple_t
	{
		Triple_t() = default;
		Triple_t(const T1& init1, const T2& init2, const T3& init3) :
			first_{ init1 },
			second_{ init2 },
			third_{ init3 }
		{}
		Triple_t(const Triple_t&) = default;
		Triple_t(Triple_t&&) = default;

		T1 first_;
		T2 second_;
		T3 third_;
	};
	template <class T1, class T2, class T3>
	Triple_t<T1, T2, T3> make_triple(const T1& elem1, const T2& elem2, const T3& elem3)
	{
		return std::move(Triple_t<T1, T2, T3>{elem1, elem2, elem3});
	}
	////////////////////////////////////////////////////////////////////////////////
}




using Triangle_t     = geometry::Polygon_t<3>;
//size_t - number of current triangle
//bool - is this triangle was used
using ListTriangle_t = std::list<geometry::Triple_t<Triangle_t*, size_t, bool>>;
#define NOT_USED_NODE false
#define USED_NODE     true


namespace geometry {
	////////////////////////
	//	return orthogonal line to triangle)
	////////////////////////
	geometry::Line3_t get_ort(const Triangle_t& triangle);

	////////////////////////
	//	sort to left and right sides of triangle
	////////////////////////
	void sort_triangles(ListTriangle_t& main, ListTriangle_t& left, ListTriangle_t& right);

	////////////////////////
	//	return 1 if triangles intersect
	////////////////////////
	int InterTriangle2(const geometry::Polygon_t<2>& first, const geometry::Polygon_t<2>& second);

	////////////////////////
	//	the same - in 3d
	////////////////////////
	int InterTriangle3(const Triangle_t& first, const Triangle_t& second);

	////////////////////////
	//	main functions for many triangles - algorithm O(n * log(n))
	////////////////////////
	int CountIntersect(ListTriangle_t&& triangles, std::set<size_t>& inter_nodes);
}

int main()
{
#define file std::cin
	/*
	std::ifstream file;
	file.open("10000.1.txt", std::ios_base::in);
	if (!file.is_open())
		ERROR("File wasn't opened correctly!");
		*/
	size_t size = 0;
	file >> size;

	std::vector<Triangle_t> triangles{ size };
	for (size_t i = 0; i < size; ++i) {
		triangles[i].input_data(file, 3);
	}

	ListTriangle_t to_function;
	for (size_t i = 0; i < size; ++i) {
		to_function.push_back(std::move(make_triple(&(triangles[i]), i, NOT_USED_NODE)));
		if (i == 601)
			to_function.back().first_->dump();
		if (i == 5596)
			to_function.back().first_->dump();
	}
	std::cout << "\n\n";


	std::set<size_t> inter_nodes;
	geometry::CountIntersect(std::move(to_function), inter_nodes);

	
	//for (const auto& elem : inter_nodes)
	//	std::cout << elem << std::endl;
}



//1 if intersect
int geometry::InterTriangle2(const Polygon_t<2>& first, const Polygon_t<2>& second)
{
	for (int i = 0; i < 3; ++i) {
		Line_t cur_line1(first[i], first[(i + 1) % 3]);

		bool output = false;
		for (int j = 0; j < 3; ++j) {
			output = cur_line1.is_one_side(first[(i + 2) % 3], second[j]);
			if (output == true)
				break;
		}
		if (output == false)
			return false;

		Line_t cur_line2(second[i], second[(i + 1) % 3]);
		output = false;
		for (int j = 0; j < 3; ++j) {
			output = cur_line2.is_one_side(second[(i + 2) % 3], first[j]);
			if (output == true)
				break;
		}
		if (output == false)
			return false;
	}
	return true;
}
int geometry::InterTriangle3(const Triangle_t& first, const Triangle_t& second)
{
	auto first_points = first.get_points();
	auto secon_points = second.get_points();

	//check projection of Triangles on the main planes
	//if every triangles are intersects -> our intersects
	for (int i = 0; i < 3; ++i) {
		if (!InterTriangle2(first.get_projection(i), second.get_projection(i)))
			return false;
	}
	return true;
}


geometry::Line3_t geometry::get_ort(const Triangle_t& triangle)
{
	Point_t<3> vec_1 = triangle[1] - triangle[0];
	Point_t<3> vec_2 = triangle[2] - triangle[0];
	Point_t<3> out_ort = { vec_1[1] * vec_2[2] - vec_1[2] * vec_2[1],
						  -vec_1[0] * vec_2[2] + vec_1[2] * vec_2[0],
						   vec_1[0] * vec_2[1] - vec_1[1] * vec_2[0] };

	return std::move(Line3_t(triangle[0], out_ort + triangle[0]));
}

void geometry::sort_triangles(ListTriangle_t& main, ListTriangle_t& left, ListTriangle_t& right)
{
	auto iter = main.begin();
	Line3_t main_ort = get_ort(*(iter->first_));
	iter++;

	while (iter != main.end()) {
		help_func::sign cur_sign = help_func::get_sign(main_ort.scal_mult(iter->first_->operator[](0)));

		if (cur_sign == help_func::sign::ZERO) {
			iter++;
			continue;
		}

		bool should_be_deleted = true;
		for (int i = 1; i < 3; ++i) {
			if (cur_sign != help_func::get_sign(main_ort.scal_mult(iter->first_->operator[](i)))) {
				should_be_deleted = false;
				break;
			}
		}

		auto erase_iter = iter;
		iter++;

		if (should_be_deleted) {

			if (cur_sign == help_func::sign::MINUS)
				left.push_back(*erase_iter);
			else
				right.push_back(*erase_iter);
			main.erase(erase_iter);
		}
	}
}
int geometry::CountIntersect(ListTriangle_t&& triangles, std::set<size_t>& inter_nodes)
{
	size_t size_list = triangles.size();
	if (triangles.empty() || size_list == 1)
		return 0;
	if (size_list == 2) {
		if (InterTriangle3(*(triangles.front().first_), *(triangles.back().first_))) {
			inter_nodes.insert(triangles.front().second_);
			inter_nodes.insert(triangles.back ().second_);
			std::cout << 1 << "\t" << triangles.front().second_ << "\t" << triangles.back().second_ << std::endl;
			return 1;
		}
		return 0;
	}

	//common case
	ListTriangle_t left_side;
	ListTriangle_t right_side;
	sort_triangles(triangles, left_side, right_side);

	//UNCOMIIT IT TO CHECK LOG_N 
	//How can we seperate data
	/*
	for (const auto& elem : triangles) {
		elem->dump();
		std::cout << std::endl;
	}
	std::cout << "\nLeft sight:\n";
	for (const auto& elem : left_side) {
		elem->dump();
		std::cout << std::endl;
	}
	std::cout << "\nRight sight:\n";
	for (const auto& elem : right_side) {
		elem->dump();
		std::cout << std::endl;
	}*/

	if (left_side.empty() || right_side.empty()) {
		
		left_side.splice(left_side.cend(), std::move(right_side));

		ListTriangle_t::iterator Pol = triangles.begin();
		int result = 0;

		for (auto iter = std::next(triangles.begin()); iter != triangles.end(); ++iter) {
			if (InterTriangle3(*(Pol->first_), *(iter->first_))) {
				result++;
				Pol->third_ = true;
				iter->third_ = true;
				std::cout << Pol->second_ << "\t" << iter->second_ << std::endl;
			}
		}

		if (Pol->third_)
			inter_nodes.insert(Pol->second_);

		triangles.erase(triangles.begin());
		triangles.splice(triangles.cend(), std::move(left_side));

		return result + CountIntersect(std::move(triangles), inter_nodes);
	}	

	//move elements from triangle to left_side and right_side
	left_side.insert(left_side.cend(), triangles.begin(), triangles.end());
	right_side.splice(right_side.cend(), std::move(triangles));

	return CountIntersect(std::move(left_side), inter_nodes) + CountIntersect(std::move(right_side), inter_nodes);
}