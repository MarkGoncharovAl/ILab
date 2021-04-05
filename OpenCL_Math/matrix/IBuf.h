/****************************************************************************************
 *
 * IBuf.h
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

//
/// IBuff_t
///======================================================================================
/// this class is responsible for allocating and freeing RAW memory(without calling
/// constructors).
///
/// Used as a way to provide a basic exception guarantee. It is guaranteed that when an
/// exception is thrown for a child class, all memory will be freed, and the constructed
/// objects will be correctly destroyed.
///
///
/// How to use:
/// - inherit your class, which should manage memory, from IBuff_t.
/// - Stretch to the namespace of its class m_used, m_data and m_size.
/// - You can now use m_data to construct objects. (if m_size != 0).
///
/// IMPORTANT: objects must be constructed tightly in memory, starting from the very
/// beginning of m_data! (prototype of the required call:
/// 'new(m_data + m_used) Type (args...);'
///
/// One constructed object -> m_used++.
///
/// Do not free or allocate the memory in m_data yourself! The Buff_t class is
/// responsible for this
///======================================================================================
///======================================================================================
//

#pragma once

#include <exception>
#include <iostream>
#include <string>

namespace matrix
{

	/// \brief
	///     Allocates raw memory for subsequent object construction.
	/// \tparam
	///     T - type of data
	/// \note
	///     For proper operation, it is necessary to construct the objects in order (compact).
    ///     For each constructed object m_used++.
	template< typename T >
	class IBuff_t
	{
	public:

	    // Copying is prohibited because an exception may be thrown when copying objects
		IBuff_t             (const IBuff_t&) = delete;
		IBuff_t& operator = (const IBuff_t&) = delete;

	protected:

		T*     m_data = nullptr;

		//array size (raw memory)
		size_t m_size = 0;

		//number of constructed objects
		size_t m_used = 0;

	protected:

		IBuff_t() = default;
		
		explicit IBuff_t(size_t size)
			: m_data((size == 0) ? nullptr : static_cast< T* >(::operator new[](sizeof(T) * size)))
			, m_size(size)
			, m_used(0)
		{}

		~IBuff_t()
		{
			for (size_t k = 0; k < m_used; ++k)
			{
				(m_data + k)->~T();
			}
			::operator delete[] (m_data);
		}

		IBuff_t(IBuff_t&& that) noexcept
			: IBuff_t()
		{
			swap(that);
		}

		IBuff_t& operator = (IBuff_t&& that) noexcept
		{
			IBuff_t tmp(std::move(that));
			swap(tmp);
		}
		
		size_t size() const noexcept { return m_size; }

		void swap(IBuff_t& that) noexcept
		{
			std::swap(m_data, that.m_data);
			std::swap(m_size, that.m_size);
			std::swap(m_used, that.m_used);
		}
	};//class IBuff_t


}//namespace matrix