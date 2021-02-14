#pragma once
#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <array>
#include <vector>
#include <list>
#define double long double

constexpr double approx = 0.000000000000000001;


namespace help_func {

	/////////////////////////////////////////////////////
	// Use ERROR(const std::string& info) instead
	// Info that programm can't run futhermore
	/////////////////////////////////////////////////////
	[[noreturn]] inline void print_error(const std::string& info, std::size_t LINE, const std::string& file) {
		std::cout << "\nMisatake was found in file: " << file << std::endl << info;
		std::cout << "\nIt was in line " << LINE << std::endl;
		exit(EXIT_FAILURE);
	}
	/////////////////////////////////////////////////////
	// Use WARNING(const std::string& info) instead
	// Info about probably mistakes in programm
	/////////////////////////////////////////////////////
	inline void print_warning(const std::string& info, std::size_t LINE, const std::string& file) {
		std::cout << "\nWarning was found!\n" << file << std::endl << info;
		std::cout << "\nIt was in line " << LINE << std::endl;
	}

	enum class sign { MINUS, ZERO, PLUS };
	sign get_sign(double num)
	{
		if (std::fabs(num) < approx)
			return sign::ZERO;

		if (num > 0)
			return sign::PLUS;

		return sign::MINUS;
	}
}
#define ERROR(a)   help_func::print_error  (a, __LINE__, __FILE__)
#define WARNING(a) help_func::print_warning(a, __LINE__, __FILE__)



//MAIN PART


namespace geometry {

	template <size_t dim_>
	class Point_t
	{
	public:

		Point_t()                                            noexcept;
		Point_t(std::initializer_list<double> init)          noexcept;
		Point_t(const Point_t& that)                         noexcept;
		Point_t(Point_t&& that)                              noexcept;

		double operator [](int num) const                     noexcept { return coords_[num]; }
		void operator =  (const Point_t& that)               noexcept;
		void operator =  (Point_t&& that)               noexcept;
		bool operator == (const Point_t& that) const         noexcept;
		Point_t<dim_> operator - (const Point_t<dim_>& that) const    noexcept;
		Point_t<dim_> operator + (const Point_t<dim_>& that) const    noexcept;
		void add_coord(int ort, double num);

		////////////
		//	check: can elem be changed
		////////////
		explicit operator bool() const noexcept;

		void dump() const;

	private:
		std::array<double, dim_> coords_;
	};
}


template <size_t dim_>
geometry::Point_t<dim_>::Point_t() noexcept :
	coords_{}
{
	for (double& elem : coords_)
		elem = std::numeric_limits<double>::quiet_NaN();
}
template <size_t dim_>
geometry::Point_t<dim_>::Point_t(std::initializer_list<double> init) noexcept
{
	if (init.size() != dim_)
		ERROR("Can't create point!");
	
	typename std::array<double, dim_>::iterator cur_iter = coords_.begin();
	for (const auto& elem : init) {
		*cur_iter = elem;
		cur_iter++;
	}
}
template <size_t dim_>
geometry::Point_t<dim_>::Point_t(const Point_t<dim_>& that)                  noexcept :
	coords_{ that.coords_ }
{}
template <size_t dim_>
geometry::Point_t<dim_>::Point_t(Point_t&& that)                              noexcept :
	coords_{std::move(that.coords_)}
{}
template <size_t dim_>
geometry::Point_t<dim_>::operator bool() const noexcept
{
	for (auto elem : coords_)
		if (!std::isfinite(elem))
			return false;
	
	return true;
}
template <size_t dim_>
bool geometry::Point_t<dim_>::operator == (const Point_t<dim_>& that) const noexcept
{
	for (int i = 0; i < coords_.size(); ++i)
		if (std::fabs(coords_[i] - that[i]) > approx)
			return false;

	return true;
}
template <size_t dim_>
void geometry::Point_t<dim_>::operator =  (const Point_t<dim_>& that)                noexcept
{
	coords_ = that.coords_;
}
template <size_t dim_>
void geometry::Point_t<dim_>::operator =  (      Point_t<dim_>&& that)               noexcept
{
	coords_ = std::move(that.coords_);
}
template <size_t dim_>
geometry::Point_t<dim_> geometry::Point_t<dim_>::operator - (const Point_t<dim_>& that) const    noexcept
{
	Point_t<dim_> output;
	for (size_t i = 0; i < dim_; ++i)
		output.coords_[i] = coords_[i] - that.coords_[i];
	return std::move(output);
}
template <size_t dim_>
geometry::Point_t<dim_> geometry::Point_t<dim_>::operator + (const Point_t<dim_>& that) const    noexcept
{
	Point_t<dim_> output;
	for (size_t i = 0; i < dim_; ++i)
		output.coords_[i] = coords_[i] + that.coords_[i];
	return std::move(output);
}
template <size_t dim_>
void geometry::Point_t<dim_>::add_coord(int ort, double num)
{
	if (ort >= dim_)
		ERROR("Can't change coordinates of point!");

	coords_[ort] = num;
}
template <size_t dim_>
void geometry::Point_t<dim_>::dump() const
{
	for (double num : coords_)
		std::cout << num << "\t";
	std::cout << std::endl;
}












namespace geometry {
	template <size_t dim_>
	class Polygon_t
	{
	public:
		Polygon_t() noexcept {}
		Polygon_t(std::initializer_list<Point_t<dim_>> init) noexcept;
		Polygon_t(Polygon_t<dim_>&& that) noexcept;

		Point_t<dim_> operator [](int num) const noexcept { return points_[num]; }

		std::vector<Point_t<dim_>> get_points() const noexcept { return points_; }
		Polygon_t<dim_ - 1>        get_projection(int ort) const;
		void                       add_point(int ort, const Point_t<dim_>& new_data);
		void                       add_new_point(const Point_t<dim_>& new_data);

		bool                       is_valid() const noexcept;
		void                       input_data(std::istream& in, size_t count_nodes);
		void                       input_data(std::ifstream& in, size_t count_nodes);

		void dump() const;

	private:
		std::vector<Point_t<dim_>> points_;
	};
}

template <size_t dim_>
geometry::Polygon_t<dim_>::Polygon_t(std::initializer_list<Point_t<dim_>> init) noexcept :
	points_{init}
{}
template <size_t dim_>
geometry::Polygon_t<dim_ - 1> geometry::Polygon_t<dim_>::get_projection(int ort) const
{
	if (ort >= dim_)
		ERROR("Can't find projection");

	Polygon_t<dim_ - 1> proj;

	for (const auto& point : points_) {
		Point_t<dim_ - 1> pt;
		int copy_ort = 0;
		int cur_ort  = 0;

		while (copy_ort < dim_) {
			if (copy_ort == ort) {
				copy_ort++;
				continue;
			}
			pt.add_coord(cur_ort, point[copy_ort]);
			copy_ort++;
			cur_ort++;
		}

		proj.add_new_point(pt);
	}
	return std::move(proj);
}
template <size_t dim_>
geometry::Polygon_t<dim_>::Polygon_t(Polygon_t<dim_>&& that) noexcept :
	points_{std::move(that.points_)}
{}
template <size_t dim_>
void geometry::Polygon_t<dim_>::add_point(int ort, const Point_t<dim_>& new_data)
{
	points_[ort] = new_data;
}
template <size_t dim_>
void geometry::Polygon_t<dim_>::add_new_point(const Point_t<dim_>& new_data)
{
	points_.push_back(new_data);
}
template <size_t dim_>
void geometry::Polygon_t<dim_>::dump() const
{
	for (const auto& elem : points_)
		elem.dump();
}
template <size_t dim_>
bool geometry::Polygon_t<dim_>::is_valid() const noexcept
{
	if (points_.empty())
		return false;

	for (const Point_t<dim_>& pt : points_) {
		if (!pt) //check valid of point
			return false;
	}
	return true;
}
template <size_t dim_>
void geometry::Polygon_t<dim_>::input_data(std::istream& in, size_t count_nodes)
{
	points_.resize(count_nodes);

	double new_coord = 0;
	for (size_t i = 0; i < count_nodes; ++i) {
		
		for (int j = 0; j < 3; ++j) {

			in >> new_coord;
			if (!in.good())
				ERROR("Can't read data of point");

			points_[i].add_coord(j, new_coord);
		}
	}
}
template <size_t dim_>
void geometry::Polygon_t<dim_>::input_data(std::ifstream& in, size_t count_nodes)
{
	points_.resize(count_nodes);

	double new_coord = 0;
	for (size_t i = 0; i < count_nodes; ++i) {

		for (int j = 0; j < 3; ++j) {

			in >> new_coord;
			if (!in.good())
				ERROR("Can't read data of point");

			points_[i].add_coord(j, new_coord);
		}
	}
}










namespace geometry {
	class Line_t
	{
	public:

		Line_t(const Point_t<2>& first, const Point_t<2>& second) noexcept;

		////////
		//	check: if points lhs and rhs on one side of our line
		////////
		bool is_one_side(const Point_t<2>& lhs, const Point_t<2>& rhs) const;

	private:
		Point_t<2> vector_;
		Point_t<2> point_;

		double vec_mult(const Point_t<2>& lhs, const Point_t<2>& rhs) const;

	};
}

geometry::Line_t::Line_t(const Point_t<2>& first, const Point_t<2>& second) noexcept :
	vector_{ second - first },
	point_ { first }
{}
bool geometry::Line_t::is_one_side(const Point_t<2>& lhs, const Point_t<2>& rhs) const
{
	return help_func::get_sign(vec_mult(lhs - point_, vector_)) == help_func::get_sign(vec_mult(rhs - point_, vector_));
}
double geometry::Line_t::vec_mult(const Point_t<2>& lhs, const Point_t<2>& rhs) const
{
	return lhs[0] * rhs[1] - lhs[1] * rhs[0];
}



namespace geometry {
	class Line3_t
	{
	public:
		static constexpr double approx = 0.000001;

		Line3_t(const Point_t<3>& first, const Point_t<3>& second) noexcept;
		double scal_mult(const Point_t<3>& pt) const noexcept;

		void set_point(const Point_t<3>& new_point) { point_ = new_point; }
		void set_vector(const Point_t<3>& new_vec) { vector_ = new_vec; }
		void dump() const;

	private:
		Point_t<3> vector_;
		Point_t<3> point_;
	};
}

geometry::Line3_t::Line3_t(const Point_t<3>& first, const Point_t<3>& second) noexcept :
	vector_{ second - first },
	point_{ first }
{}
double geometry::Line3_t::scal_mult(const Point_t<3>& pt) const noexcept
{
	Point_t<3> new_vec = pt - point_;
	
	double result = 0.0;
	for (int i = 0; i < 3; ++i) {
		result += new_vec[i] * vector_[i];
	}
	
	return result;
}
void geometry::Line3_t::dump() const
{
	vector_.dump();
	point_.dump();
}
