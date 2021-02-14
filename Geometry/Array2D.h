#pragma once
#include <iostream>
#include <functional>

namespace lib {

	//For user help
	[[noreturn]]
	inline void print_error  (std::string info, std::string FILE, std::size_t LINE) noexcept;
	inline void print_warning(std::string info, std::string FILE, std::size_t LINE) noexcept;



	template <typename T>
	class Array2D {
	public:

		Array2D(Array2D&& other)                                                 noexcept;
		Array2D(const Array2D& other)                                            noexcept;

		Array2D(std::size_t max_x,         std::size_t max_y)                    noexcept;
		Array2D(std::unique_ptr<T>&& data, std::size_t max_x, std::size_t max_y) noexcept;
		Array2D(const T* data,             std::size_t max_x, std::size_t max_y) noexcept;

		T* operator []  (std::size_t dist)                                       noexcept;
		T* operator []  (int dist)                                               noexcept;
		T* operator *   ()                                                       noexcept { return data_.get(); }

		void operator = (const T* other)                                         noexcept;

		size_t get_size_x() const noexcept { return max_x_; }
		size_t get_size_y() const noexcept { return max_y_; }

		void dump() const noexcept;

		//iterator work
		//------------------------------------------------------------------------------------------------------
		class basic_iterator
		{
		public:
			basic_iterator()                      noexcept;
			basic_iterator(Array2D<T>* init_data) noexcept;
			basic_iterator(T* init_data)          noexcept;

			explicit operator bool() const noexcept { return data_ != nullptr; }

			void operator ++ ()    noexcept { data_++; }
			void operator ++ (int) noexcept { data_++; }
			void operator -- ()    noexcept { data_--; }
			void operator -- (int) noexcept { data_--; }

			void operator += (int dist)    noexcept { data_ + dist; }
			void operator += (size_t dist) noexcept { data_ + dist; }
			void operator -= (int dist)    noexcept { data_ - dist; }
			void operator -= (size_t dist) noexcept { data_ - dist; }

		protected:
			T* data_;
		};
		class iterator : 
			public basic_iterator
		{
		public:
			iterator()                      noexcept :
				basic_iterator {}
			{}
			iterator(Array2D<T>* init_data) noexcept :
				basic_iterator { init_data }
			{}
			iterator(T* init_data)          noexcept :
				basic_iterator { init_data }
			{}

			bool operator == (const lib::Array2D<T>::iterator& other) const noexcept { return this->data_ == other.data_; }
			bool operator != (const lib::Array2D<T>::iterator& other) const noexcept { return this->data_ != other.data_; }

			lib::Array2D<T>::iterator operator + (int dist)    const noexcept { return std::move(lib::Array2D<T>::iterator(this->data_ + dist)); }
			lib::Array2D<T>::iterator operator + (size_t dist) const noexcept { return std::move(lib::Array2D<T>::iterator(this->data_ + dist)); }
			lib::Array2D<T>::iterator operator - (int dist)    const noexcept { return std::move(lib::Array2D<T>::iterator(this->data_ - dist)); }
			lib::Array2D<T>::iterator operator - (size_t dist) const noexcept { return std::move(lib::Array2D<T>::iterator(this->data_ - dist)); }

			T& operator * () noexcept { return *this->data_; }
		};
		class const_iterator : 
			public basic_iterator
		{
		public:
			const_iterator()                      noexcept :
				basic_iterator {}
			{}
			const_iterator(Array2D<T>* init_data) noexcept :
				basic_iterator { init_data }
			{}
			const_iterator(T* init_data)          noexcept :
				basic_iterator { init_data }
			{}

			bool operator == (const lib::Array2D<T>::const_iterator& other) const noexcept { return this->data_ == other.data_; }
			bool operator != (const lib::Array2D<T>::const_iterator& other) const noexcept { return this->data_ != other.data_; }

			lib::Array2D<T>::const_iterator operator + (int dist)    const noexcept { return std::move(lib::Array2D<T>::const_iterator(this->data_ + dist)); }
			lib::Array2D<T>::const_iterator operator + (size_t dist) const noexcept { return std::move(lib::Array2D<T>::const_iterator(this->data_ + dist)); }
			lib::Array2D<T>::const_iterator operator - (int dist)    const noexcept { return std::move(lib::Array2D<T>::const_iterator(this->data_ - dist)); }
			lib::Array2D<T>::const_iterator operator - (size_t dist) const noexcept { return std::move(lib::Array2D<T>::const_iterator(this->data_ - dist)); }

			const T&  operator * () const noexcept { return *this->data_; }
		};
		class safe_iterator :
			public basic_iterator
		{
		public:
			safe_iterator(T* begin, T* end)                        noexcept :
				begin_{ begin },
				end_  { end },
				basic_iterator {}
			{}
			safe_iterator(T* begin, T* end, Array2D<T>* init_data) noexcept :
				begin_{ begin },
				end_  { end },
				basic_iterator { init_data }
			{}
			safe_iterator(T* begin, T* end, T* init_data)          noexcept :
				begin_{ begin },
				end_  { end },
				basic_iterator { init_data }
			{}

			bool operator == (const lib::Array2D<T>::safe_iterator& other) const noexcept { return this->data_ == other.data_; }
			bool operator != (const lib::Array2D<T>::safe_iterator& other) const noexcept { return this->data_ != other.data_; }

			lib::Array2D<T>::safe_iterator operator + (int dist)    const noexcept { return std::move(lib::Array2D<T>::safe_iterator(begin_, end_, this->data_ + dist)); }
			lib::Array2D<T>::safe_iterator operator + (size_t dist) const noexcept { return std::move(lib::Array2D<T>::safe_iterator(begin_, end_, this->data_ + dist)); }
			lib::Array2D<T>::safe_iterator operator - (int dist)    const noexcept { return std::move(lib::Array2D<T>::safe_iterator(begin_, end_, this->data_ - dist)); }
			lib::Array2D<T>::safe_iterator operator - (size_t dist) const noexcept { return std::move(lib::Array2D<T>::safe_iterator(begin_, end_, this->data_ - dist)); }

			T& operator * () noexcept 
			{
				if (this->data_ < begin_ || this->data_ >= end_)
					print_error("Tried to go away from array!", __FILE__, __LINE__);
				return *this->data_; 
			}
			bool is_out_range() const noexcept { return this->data_ < begin_ || this->data_ >= end_; }

		private:
			T* begin_;
			T* end_;

			safe_iterator() noexcept;
		};


		Array2D<T>::iterator       begin () noexcept       { return std::move(lib::Array2D<T>::iterator      (data_.get())); }
		Array2D<T>::iterator       end   () noexcept       { return std::move(lib::Array2D<T>::iterator      (data_.get() + max_x_ * max_y_)); }
		Array2D<T>::const_iterator cbegin() const noexcept { return std::move(lib::Array2D<T>::const_iterator(data_.get())); }
		Array2D<T>::const_iterator cend  () const noexcept { return std::move(lib::Array2D<T>::const_iterator(data_.get() + max_x_ * max_y_)); }
		Array2D<T>::safe_iterator  sbegin() noexcept       { return std::move(lib::Array2D<T>::safe_iterator (data_.get(), data_.get() + max_x_ * max_y_, data_.get())); }
		Array2D<T>::safe_iterator  send  () noexcept       { return std::move(lib::Array2D<T>::safe_iterator (data_.get(), data_.get() + max_x_ * max_y_, data_.get() + max_x_ * max_y_)); }

		// + I overloaded functions std::begin() and std::end() for this iterators
		// It means that you can use this syntaxis
		// for (auto& it : Array)
		// { some work with "it" }
		//------------------------------------------------------------------------------------------------------

	private:
		std::unique_ptr<T> data_;
		std::size_t        max_x_;
		std::size_t        max_y_;

		//in order to ban access
		Array2D() noexcept;
	};
}

template <typename T>
lib::Array2D<T>::Array2D(Array2D&& other)                                                 noexcept :
	data_ (std::move(other.data_)),
	max_x_(std::move(other.max_x_)),
	max_y_(std::move(other.max_y_))
{}
template <typename T>
lib::Array2D<T>::Array2D(const Array2D& other)                                            noexcept :
	data_ (new T[other.max_x_ * other.max_y_]),
	max_x_(other.max_x_),
	max_y_(other.max_y_)
{
	this->operator=(other.data_.get());
}

template <typename T>
lib::Array2D<T>::Array2D(std::size_t max_x, std::size_t max_y)                            noexcept :
	data_ (new T[max_x * max_y]),
	max_x_(max_x),
	max_y_(max_y)
{}
template <typename T>
lib::Array2D<T>::Array2D(std::unique_ptr<T>&& data, std::size_t max_x, std::size_t max_y) noexcept :
	data_ (std::move(data)),
	max_x_(max_x),
	max_y_(max_y)
{}
template <typename T>
lib::Array2D<T>::Array2D(const T* data, std::size_t max_x, std::size_t max_y)             noexcept :
	data_ (new T[max_x * max_y]),
	max_x_(max_x),
	max_y_(max_y)
{
	this->operator = (data);
}




template <typename T>
T* lib::Array2D<T>::operator[] (std::size_t dist) noexcept
{
	if (dist >= max_x_) {
		print_error("Tried to go away from Array2D!", __FILE__, __LINE__);
	}
	return (data_.get() + dist * max_y_);
}
template <typename T>
T* lib::Array2D<T>::operator[]   (int       dist) noexcept
{
	std::size_t dist_t = static_cast<std::size_t> (dist);

	if (dist < 0 || dist_t >= max_x_) {
		print_error("Tried to go away from Array2D!", __FILE__, __LINE__);
	}
	return (data_.get() + dist_t * max_y_);
}

template <typename T>
void lib::Array2D<T>::operator = (const T* other) noexcept
{
	T* cur_data = data_.get();
	std::size_t full_size = max_x_ * max_y_;

	for (std::size_t i = 0; i < full_size; ++i) {
		*cur_data = *other;
		cur_data++;
		other++;
	}
}

template <typename T>
void lib::Array2D<T>::dump() const noexcept
{
	T* cur_data = data_.get();
	size_t cur_x = 1;
	size_t cur_y = 0;

	while (cur_y != max_x_) {

		std::cout << *cur_data;
		if (cur_x < max_x_) {
			std::cout << "\t";
			cur_x++;
		}
		else {
			cur_y++;
			std::cout << "\n";
			cur_x = 1;
		}

		cur_data++;
	}
}




template <class T>
lib::Array2D<T>::basic_iterator::basic_iterator() noexcept :
	data_ { nullptr } 
{}
template <class T>
lib::Array2D<T>::basic_iterator::basic_iterator(Array2D<T>* init_data) noexcept :
	data_{ init_data->data_ }
{}
template <class T>
lib::Array2D<T>::basic_iterator::basic_iterator(T* init_data) noexcept :
	data_{ init_data }
{}





[[noreturn]] 
inline void lib::print_error  (std::string info, std::string FILE, std::size_t LINE) noexcept
{
	std::cout << std::endl << info << std::endl;
	std::cout << "Mistake was found in file " << FILE;
	printf("\nMistake was found in line %ld\n", LINE);
	exit(EXIT_FAILURE);
}
inline void lib::print_warning(std::string info, std::string FILE, std::size_t LINE) noexcept
{
	std::cout << std::endl << info << std::endl;
	std::cout << "Warning was found in file %ld\n" << FILE;
	printf("\nWarning was found in line %ld\n", LINE);
}



namespace std
{
	template <typename T>
	typename lib::Array2D<T>::iterator begin(const lib::Array2D<T>& obj)
	{
		return obj.begin();
	}

	template <typename T>
	typename lib::Array2D<T>::iterator end  (const lib::Array2D<T>& obj)
	{
		return obj.end();
	}
}