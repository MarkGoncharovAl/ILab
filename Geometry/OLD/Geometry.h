#pragma once

#include <iostream>
#include <fstream>
#include <cmath>
#include <limits>
#include <array>
#include <vector>
#include <list>

#include "Array2D.h"


namespace help_functions
{
	/////////////////////////////////////////////////////
	// Use ERROR(const std::string& info) instead
	// Info that programm can't run futhermore
	//	Example: ERORR("File wasn't found!");
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
	
	template <typename T>
	sign get_sign(T num);

	template <typename T>
	T Determinant(lib::Array2D<T>& arr);
}
//USE IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define ERROR(a)   help_functions::print_error  (a, __LINE__, __FILE__)
#define WARNING(a) help_functions::print_warning(a, __LINE__, __FILE__)
//USE IT!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!





//It will be the main namespace
namespace geometry
{
	using coord_type = long double;
	
	constexpr long double approx = 0.0000000001;


	// use _ after member in class
	template <size_t dim_>
	class Point_t
	{
	public:
		Point_t();
		Point_t(const Point_t<dim_>&)  = default;
		Point_t(      Point_t<dim_>&&) = default;

		Point_t(std::initializer_list<coord_type> init);

		double   operator []  (int    num)                 const { return coord_[num]; }
		double   operator []  (size_t num)                 const { return coord_[num]; }

		void     operator =   (const Point_t<dim_>&  that)       { coord_ = that.coord_; }
		void     operator =   (      Point_t<dim_>&& that)       { coord_ = std::move(that.coord_); }

		/////////////
		//	compare two points
		////////////
		bool     operator ==  (const Point_t<dim_>&  that) const;

		/////////////
		//	add coord 
		//	first ort is 0, 1, 2...
		////////////
		void     set_coord    (int ort, double num);

		/////////////
		//	check validation of point, operator bool the same
		////////////
		bool     is_valid     ()                           const;
		explicit operator bool()                           const { return is_valid(); };

		/////////////
		//	Print full statistic of point
		//	Using:
		//	dump(std::cout);
		/////////////
		void     dump         (std::ostream&  out, const std::string& extra_info = "") const;
		void     dump         (std::ofstream& out, const std::string& extra_info = "") const;

	private:
		std::array<coord_type, dim_> coord_;
	};

	template <size_t dim_>
	class Vector_t
	{
	public:
		Vector_t();
		Vector_t(const Vector_t<dim_>& ) = default;
		Vector_t(      Vector_t<dim_>&&) = default;

		Vector_t(std::initializer_list<coord_type> init);

		Vector_t(const Point_t<dim_>& first, const Point_t<dim_>& second);

		double   operator []  (int    num)                  const { return coord_[num]; }
		double   operator []  (size_t num)                  const { return coord_[num]; }

		/////////////
		//	add coord 
		//	first ort is 0, 1, 2...
		////////////
		void     set_coord    (int ort, double num);

		void     operator =   (const Vector_t<dim_>&  that)       { coord_ = that.coord_; }
		void     operator =   (      Vector_t<dim_>&& that)       { coord_ = std::move(that.coord_); }

		/////////////
		//	compare two vectors
		////////////
		bool     operator ==  (const Vector_t<dim_>& that)  const;

		/////////////
		//	check validation of point, operator bool the same
		////////////
		bool     is_valid     ()                            const;
		explicit operator bool()                            const { return is_valid(); };

		/////////////
		//	return projection THIS vector on INPUT
		////////////
		Vector_t<dim_> get_projection_on(const Vector_t<dim_>& base) const;

		/////////////
		//	Print full statistic of point
		//	Using:
		//	dump(std::cout);
		/////////////
		void     dump(std::ostream&  out, const std::string& extra_info = "") const;
		void     dump(std::ofstream& out, const std::string& extra_info = "") const;

	private:
		std::array<coord_type, dim_> coord_;
	};

	
	template <size_t dim_>
	class Line_t
	{
	public:
		Line_t()                     = default;
		Line_t(const Line_t<dim_>&)  = default;
		Line_t(      Line_t<dim_>&&) = default;
		Line_t(const Point_t<dim_>& init1, const Point_t <dim_>& init2);
		Line_t(const Point_t<dim_>& init1, const Vector_t<dim_>& init2);

		void set(const Point_t<dim_>& new_point, const Vector_t<dim_>& new_vec);
		
		Point_t<dim_>  get_point () const { return point_; }
		Vector_t<dim_> get_direct() const { return direct_; }

		/////////////
		//	check validation of point, operator bool the same
		////////////
		bool     is_valid     () const { return point_ && direct_; }
		explicit operator bool() const { return point_ && direct_; };

		/////////////
		//	Print full statistic of point
		//	Using:
		//	dump(std::cout);
		/////////////
		void     dump(std::ostream& out,  const std::string& extra_info = "") const;
		void     dump(std::ofstream& out, const std::string& extra_info = "") const;

	private:
		Point_t <dim_> point_;
		Vector_t<dim_> direct_;
	};

	
	class Surface3_t
	{
	public:
		Surface3_t()                   = default;
		Surface3_t(const Surface3_t&)  = default;
		Surface3_t(      Surface3_t&&) = default;

		Surface3_t(const Point_t<3>& init1, const Point_t<3>&  init2, const Point_t<3>& init3);
		Surface3_t(const Point_t<3>& init1, const Vector_t<3>& init2);
		Surface3_t(const Line_t<3>&  init);

		Point_t<3>  get_point () const { return std::move(normal_line_.get_point()); }
		Vector_t<3> get_normal() const { return std::move(normal_line_.get_direct()); }

		/////////////
		//	check validation of point, operator bool the same
		////////////
		bool     is_valid     () const { return normal_line_.is_valid(); }
		explicit operator bool() const { return normal_line_.is_valid(); };

		/////////////
		//	Print full statistic of point
		//	Using:
		//	dump(std::cout);
		/////////////
		void     dump(std::ostream& out,  const std::string& extra_info = "") const { normal_line_.dump(out, extra_info); }
		void     dump(std::ofstream& out, const std::string& extra_info = "") const { normal_line_.dump(out, extra_info); }

	private:
		Line_t<3> normal_line_;
	};



	///////////
	//	returns not valid point in mistake
	///////////
	template <size_t dim_>
	Point_t<dim_> operator + (const Point_t<dim_>& lhs, const Point_t<dim_>& rhs);
	template <size_t dim_>
	Point_t<dim_> operator - (const Point_t<dim_>& lhs, const Point_t<dim_>& rhs);
	///////////
	//	returns not valid vector in mistake
	///////////
	template <size_t dim_>
	Vector_t<dim_> operator + (const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs);
	template <size_t dim_>
	Vector_t<dim_> operator - (const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs);
	template <size_t dim_>
	Vector_t<dim_> operator * (const Vector_t<dim_>& lhs, coord_type coeff);
	template <size_t dim_>
	Vector_t<dim_> operator * (coord_type coeff,          const Vector_t<dim_>& rhs);


	template <size_t dim_>
	coord_type     ScalMult  (const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs);
	Vector_t<3>    VectorMult(const Vector_t<3>&    lhs, const Vector_t<3>&    rhs);
}









//REALIZATION
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
template <size_t dim_>
geometry::Point_t<dim_>::Point_t() :
	coord_{}
{
	//it means nums isn't avaliable
	for (coord_type& elem : coord_)
		elem = std::numeric_limits<coord_type>::quiet_NaN();
}
template <size_t dim_>
geometry::Point_t<dim_>::Point_t(std::initializer_list<coord_type> init)
{
	if (init.size() != dim_)
		ERROR("Can't create point!");

	typename std::array           <coord_type, dim_>::iterator cur_iter  = coord_.begin();
	typename std::initializer_list<coord_type>      ::iterator init_iter = init.begin();
	while (cur_iter != coord_.end() && init_iter != init.end()) {
		*cur_iter = *init_iter;
		cur_iter++;
		init_iter++;
	}
}
template <size_t dim_>
bool geometry::Point_t<dim_>::operator == (const Point_t<dim_>& that) const
{
	if (coord_.size() != that.coord_.size()) {
		WARNING("Can't compare two points correctly!");
		return false;
	}
	
	auto iter_this =      coord_.cbegin();
	auto iter_that = that.coord_.cbegin();
	
	while (iter_this != coord_.cend()) {
		if (*iter_this != *iter_that)
			return false;

		iter_this++;
		iter_that++;
	}
	return true;
}
template <size_t dim_>
void geometry::Point_t<dim_>::set_coord(int ort, double num)
{
	if (ort >= dim_ || ort < 0)
		ERROR("Can't change coordinates of point!");

	coord_[ort] = num;
}
template <size_t dim_>
bool geometry::Point_t<dim_>::is_valid() const
{
	for (const coord_type& elem : coord_)
		if (!std::isfinite(elem))
			return false;

	return true;
}
template <size_t dim_>
void geometry::Point_t<dim_>::dump(std::ostream& out, const std::string& extra_info /* = "" */) const
{
	out << "Dumping of Point " << extra_info << std::endl;
	
	int cur_ort = 1;
	for (const coord_type& num : coord_) {
		out << cur_ort << ":\t" << num << "\n";
		cur_ort++;
	}
	out << std::endl;
}
template <size_t dim_>
void geometry::Point_t<dim_>::dump(std::ofstream& out, const std::string& extra_info /* = "" */) const
{
	out << "Dumping of Point " << extra_info << std::endl;

	int cur_ort = 1;
	for (const coord_type& num : coord_) {
		out << cur_ort << ":\t" << num << "\n";
		cur_ort++;
	}
	out << std::endl;
}


template <size_t dim_>
geometry::Vector_t<dim_>::Vector_t() :
	coord_{}
{
	//it means nums isn't avaliable
	for (coord_type& elem : coord_)
		elem = std::numeric_limits<coord_type>::quiet_NaN();
}
template <size_t dim_>
geometry::Vector_t<dim_>::Vector_t(std::initializer_list<coord_type> init)
{
	if (init.size() != dim_)
		ERROR("Can't create vector!");

	typename std::array           <coord_type, dim_>::iterator cur_iter = coord_.begin();
	typename std::initializer_list<coord_type>      ::iterator init_iter = init.begin();
	while (cur_iter != coord_.end() && init_iter != init.end()) {
		*cur_iter = *init_iter;
		cur_iter++;
		init_iter++;
	}
}
template <size_t dim_>
geometry::Vector_t<dim_>::Vector_t(const Point_t<dim_>& first, const Point_t<dim_>& second) :
	coord_{}
{
	if (first && second) {
		for (size_t i = 0; i < dim_; ++i)
			coord_[i] = second[i] - first[i];
	}
	else
		WARNING("Vector wasn't created by two points properly");
}
template <size_t dim_>
bool geometry::Vector_t<dim_>::is_valid() const
{
	for (const coord_type& elem : coord_)
		if (!std::isfinite(elem))
			return false;

	return true;
}
template <size_t dim_>
bool geometry::Vector_t<dim_>::operator == (const Vector_t<dim_>& that) const
{
	if (coord_.size() != that.coord_.size()) {
		WARNING("Can't compare two points correctly!");
		return false;
	}

	auto iter_this = coord_.cbegin();
	auto iter_that = that.coord_.cbegin();

	while (iter_this != coord_.cend()) {
		if (*iter_this != *iter_that)
			return false;

		iter_this++;
		iter_that++;
	}
	return true;
}
template <size_t dim_>
void geometry::Vector_t<dim_>::set_coord(int ort, double num)
{
	if (ort >= dim_ || ort < 0)
		ERROR("Can't change coordinates of point!");

	coord_[ort] = num;
}
template <size_t dim_>
geometry::Vector_t<dim_> geometry::Vector_t<dim_>::get_projection_on(const Vector_t<dim_>& base) const
{
	return std::move(base * (ScalMult(*this, base) / ScalMult(base, base)));
}
template <size_t dim_>
void geometry::Vector_t<dim_>::dump(std::ostream& out, const std::string& extra_info /* = "" */) const
{
	out << "Dumping of Vector " << extra_info << std::endl;

	int cur_ort = 1;
	for (const coord_type& num : coord_) {
		out << cur_ort << ":\t" << num << "\n";
		cur_ort++;
	}
	out << std::endl;
}
template <size_t dim_>
void geometry::Vector_t<dim_>::dump(std::ofstream& out, const std::string& extra_info /* = "" */) const
{
	out << "Dumping of Vector " << extra_info << std::endl;

	int cur_ort = 1;
	for (const coord_type& num : coord_) {
		out << cur_ort << ":\t" << num << "\n";
		cur_ort++;
	}
	out << std::endl;
}


template <size_t dim_>
geometry::Line_t<dim_>::Line_t(const Point_t<dim_>& init1, const Point_t <dim_>& init2) :
	point_ { init1 },
	direct_{ init1, init2 } 
{}
template <size_t dim_>
geometry::Line_t<dim_>::Line_t(const Point_t<dim_>& init1, const Vector_t<dim_>& init2) :
	point_ { init1 },
	direct_{ init2 }
{}
template <size_t dim_>
void geometry::Line_t<dim_>::set(const Point_t<dim_>& new_point, const Vector_t<dim_>& new_vec)
{
	point_ = new_point;
	direct_ = new_vec;
}
template <size_t dim_>
void geometry::Line_t<dim_>::dump(std::ostream& out, const std::string& extra_info /* = "" */) const
{
	point_.dump(out, extra_info);
	direct_.dump(out, extra_info);
}
template <size_t dim_>
void geometry::Line_t<dim_>::dump(std::ofstream& out, const std::string& extra_info /* = "" */) const
{
	point_.dump(out, extra_info);
	direct_.dump(out, extra_info);
}


geometry::Surface3_t::Surface3_t(const Point_t<3>& init1, const Point_t<3>& init2, const Point_t<3>& init3) :
	normal_line_{}
{
	Vector_t<3> new_norm = VectorMult(Vector_t<3>{init1, init2}, Vector_t<3>{ init1, init3 });
	normal_line_.set(init1, new_norm);
}
geometry::Surface3_t::Surface3_t(const Point_t<3>& init1, const Vector_t<3>& init2) :
	normal_line_{init1, init2}
{}
geometry::Surface3_t::Surface3_t(const Line_t<3>& init) :
	normal_line_{init}
{}



template <size_t dim_>
geometry::Point_t<dim_> geometry::operator + (const Point_t<dim_>& lhs, const Point_t<dim_>& rhs)
{
	if (lhs && rhs) {
		Point_t<dim_> output{ lhs };

		for (size_t cur_ort = 0; cur_ort < dim_; ++cur_ort)
			output.set_coord(cur_ort, output[cur_ort] + rhs[cur_ort]);

		return std::move(output);
	}

	WARNING("Points weren't added properly");
	Point_t<dim_> empty_point;
	return std::move(empty_point);
}
template <size_t dim_>
geometry::Point_t<dim_> geometry::operator - (const Point_t<dim_>& lhs, const Point_t<dim_>& rhs)
{
	if (lhs && rhs) {
		Point_t<dim_> output{ lhs };

		for (size_t cur_ort = 0; cur_ort < dim_; ++cur_ort)
			output.set_coord(cur_ort, output[cur_ort] - rhs[cur_ort]);

			return std::move(output);
	}
	
	WARNING("Points weren't added properly");
	Point_t<dim_> empty_point;
	return std::move(empty_point);
}
template <size_t dim_>
geometry::Vector_t<dim_> geometry::operator + (const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs)
{
	if (lhs && rhs) {
		Vector_t<dim_> output{ lhs };

		for (size_t cur_ort = 0; cur_ort < dim_; ++cur_ort)
			output.set_coord(cur_ort, output[cur_ort] + rhs[cur_ort]);

		return std::move(output);
	}

	WARNING("Points weren't added properly");
	Vector_t<dim_> empty_point;
	return std::move(empty_point);
}
template <size_t dim_>
geometry::Vector_t<dim_> geometry::operator - (const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs)
{
	if (lhs && rhs) {
		Vector_t<dim_> output{ lhs };

		for (size_t cur_ort = 0; cur_ort < dim_; ++cur_ort)
			output.set_coord(cur_ort, output[cur_ort] - rhs[cur_ort]);

		return std::move(output);
	}

	WARNING("Points weren't added properly");
	Vector_t<dim_> empty_point;
	return std::move(empty_point);
}
template <size_t dim_>
geometry::Vector_t<dim_> geometry::operator * (const Vector_t<dim_>& lhs, coord_type coeff)
{
	Vector_t<dim_> out_vec;
	if (lhs) {

		for (size_t i = 0; i < dim_; ++i)
			out_vec.set_coord(i, lhs[i] * coeff);

	}
	else
		WARNING("Operator * was used on not valid vector!");

	return std::move(out_vec);
}
template <size_t dim_>
geometry::Vector_t<dim_> geometry::operator * (coord_type coeff, const Vector_t<dim_>& rhs)
{
	return std::move(rhs * coeff);
}

template <size_t dim_>
geometry::coord_type geometry::ScalMult(const Vector_t<dim_>& lhs, const Vector_t<dim_>& rhs)
{
	if (lhs && rhs) {
		coord_type output = 0;
		for (size_t i = 0; i < dim_; ++i) {
			output += lhs[i] * rhs[i];
		}
		return output;
	}
	WARNING("ScalMult was using on not valid vectors");
	return 0;
}
geometry::Vector_t<3> geometry::VectorMult(const Vector_t<3>& lhs, const Vector_t<3>& rhs)
{
	Vector_t<3> out_vector;

	if (lhs && rhs) {
		for (size_t i = 0; i < 3; ++i) {

			lib::Array2D<coord_type> arr(2, 2);
			coord_type sign = 1;
			if (i % 2 != 0)
				sign = -1;

			size_t cur_ort = 0;
			size_t copy_ort = 0;
			while (copy_ort < 3) {
				if (copy_ort == i) {
					copy_ort++;
					continue;
				}
				arr[0][cur_ort] = lhs[copy_ort];
				arr[1][cur_ort] = rhs[copy_ort];
				copy_ort++;
				cur_ort++;
			}

			out_vector.set_coord(i, help_functions::Determinant(arr) * sign);
		}
	}
	else
		WARNING("VectorMult was using on not valid vectors");
	
	return std::move(out_vector);
}






























template <typename T>
help_functions::sign help_functions::get_sign(T num)
{
	constexpr T approx = 0.000000000000000001;
	if (std::fabs(num) < approx)
		return sign::ZERO;

	if (num > 0)
		return sign::PLUS;

	return sign::MINUS;
}

template <typename T>
T help_functions::Determinant(lib::Array2D<T>& arr)
{
	if (arr.get_size_x() != arr.get_size_y() || arr.get_size_x() == 0)
		print_error("Can't find determinate!", __LINE__, __FILE__);

	size_t size = arr.get_size_x();
	if (size == 1)
		return arr[0][0];
	if (size == 2)
		return arr[0][0] * arr[1][1] - arr[1][0] * arr[0][1];

	T output = 0;
	for (size_t k = 0; k < size; ++k) {
		if (arr[0][k] == 0)
			continue;

		lib::Array2D<T> new_arr(size - 1, size - 1);
		T sign = 1;
		if (k % 2 == 1)
			sign = -1;


		for (size_t i = 0; i < size - 1; ++i) {
			size_t cur_ort = 0;
			size_t copy_ort = 0;

			while (copy_ort < size) {
				if (copy_ort == k) {
					copy_ort++;
					continue;
				}
				new_arr[i][cur_ort] = arr[i + 1][copy_ort];
				cur_ort++;
				copy_ort++;
			}
		}
		output += sign * arr[0][k] * Determinant(new_arr);
	}
	return output;
}