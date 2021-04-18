/*************************************************************************
 *
 * MatrixBuffer.inl
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

namespace matrix
{
    template <typename T>
    template <typename U>
    bool MatrixBuffer_t<T>::equal(const MatrixBuffer_t<U>& that_) const
    {
        bool result = true;

        if (getColumns() != that_.getColumns() || getLines() != that_.getLines()) {
            return false;
        }

        std::for_each(cbegin(), cend(), [&that_, &result](Elem<const T> elem)
        {
            result = result && (elem.val == that_.at(elem.line, elem.column));
        });

        return result;
    }

	template< typename T >
	MatrixBuffer_t<T>::MatrixBuffer_t(size_t lines, size_t columns, Order order/* = Order::Row*/)
		: IBuff_t<T>(lines* columns)
		, m_lines((columns != 0) ? lines : 0)
		, m_columns((lines != 0) ? columns : 0)
		, m_order(order)
	{
		while (m_used < m_size)
		{
			new(m_data + m_used) T();
			m_used++;
		}
	}


	template< typename T >
	MatrixBuffer_t<T>::MatrixBuffer_t(const MatrixBuffer_t& that)
		: IBuff_t<T>(that.m_size)
		, m_order(that.m_order)
		, m_lines(that.m_lines)
		, m_columns(that.m_columns)
	{
		while (m_used < that.m_used)
		{
			new(m_data + m_used) T(that.m_data[m_used]);
			m_used++;
		}
	}
	template< typename T >
	MatrixBuffer_t<T>& MatrixBuffer_t<T>::operator = (const MatrixBuffer_t& that)
	{
		MatrixBuffer_t tmp(that);
		swap(tmp);
		return *this;
	}


	template< typename T >
	template< typename U >
	MatrixBuffer_t<T>::MatrixBuffer_t(const MatrixBuffer_t< U >& that)
		: IBuff_t<T>(that.m_size)
		, m_order(that.m_order)
		, m_lines(that.m_lines)
		, m_columns(that.m_columns)
	{
		while (m_used < that.m_used)
		{
			new(m_data + m_used) T(that.m_data[m_used]);
			m_used++;
		}
	}


	template< typename T >
	MatrixBuffer_t<T>::MatrixBuffer_t(MatrixBuffer_t&& that) noexcept
		: MatrixBuffer_t()
	{
		swap(that);
	}

	template< typename T >
	MatrixBuffer_t<T>& MatrixBuffer_t<T>::operator = (MatrixBuffer_t&& that) noexcept
	{
		swap(that);
		return *this;
	}

	
	//////////////////////////////////////////////////////////////////////

	template< typename T >
	const T& MatrixBuffer_t<T>::atDefault_(size_t line, size_t column) const&
	{
		if (line >= m_lines || column >= m_columns) {
			throw std::out_of_range("Out of range valid value.");
		}

		if (m_order == Order::Row)
		{
			return *(m_data + line * m_columns + column);
		}
		else if (m_order == Order::Column)
		{
			return *(m_data + column * m_lines + line);
		}
		else { assert(0); }

		//to remove the warning
		return *(T*)nullptr;
	}


	template< typename T >
	void MatrixBuffer_t<T>::swap(MatrixBuffer_t& that) noexcept
	{
		IBuff_t<T>::swap(that);
		std::swap(m_lines, that.m_lines);
		std::swap(m_columns, that.m_columns);
		std::swap(m_order, that.m_order);
	}

	template< typename T >
	void MatrixBuffer_t<T>::clear()
	{
		*this = MatrixBuffer_t();
	}


	template< typename T >
	void MatrixBuffer_t<T>::setOrder(Order nOrder)
	{
		if (nOrder != m_order)
		{
			MatrixBuffer_t< T > tmp(m_lines, m_columns, nOrder);
			copy(tmp, *this);
			*this = std::move(tmp);
		}
	}


	template< typename T >
	template< typename U >
	/*static*/ void MatrixBuffer_t<T>::copy(MatrixBuffer_t< T >& dest, const MatrixBuffer_t< U >& src)
	{
		const size_t min_y = std::min(dest.m_lines, src.getLines());
		const size_t min_x = std::min(dest.m_columns, src.getColumns());

		MatrixBuffer_t< T > tmp(dest);

		std::for_each(tmp.begin(), tmp.end(), [&src, &min_y, &min_x](Elem<T> elem)
			{
				if (elem.line < min_y && elem.column < min_x)
				{
					elem = src.at(elem.line, elem.column);
				}
			});

		///

		dest.swap(tmp);
	}

	template <typename T>
	MatrixBuffer_t<T>& MatrixBuffer_t<T>::transpose()&
	{
		std::swap(m_columns, m_lines);
		if (m_order == Order::Column)
		{
			m_order = Order::Row;
		}
		else if (m_order == Order::Row)
		{
			m_order = Order::Column;
		}
		else { assert(0); }

		return *this;
	}

	template< typename T >
	void MatrixBuffer_t<T>::resize(size_t lines, size_t columns)
	{
		if (lines == 0 || columns == 0) {
			clear();
			return;
		}
		if (m_data == nullptr) {
			*this = std::move(MatrixBuffer_t<T>(lines, columns, m_order));
			return;
		}

		if (m_order == Order::Row && m_columns == columns && m_lines >= lines)
		{
			for (size_t l = lines; l < m_lines; l++) {
				for (size_t c = 0; c < m_columns; c++)
				{
                    at(l, c).~T();
					m_used--;
				}
			}
			m_lines = lines;
		}
		else if (m_order == Order::Row && m_columns == columns && columns * lines <= m_size)
		{
			for (size_t l = m_lines; l < lines; l++) {
				for (size_t c = 0; c < m_columns; c++)
				{
					new(&at(l, c)) T();
					m_used++;
				}
			}
			m_lines = lines;
		}
		else if (m_order == Order::Column && m_lines == lines && m_columns >= columns)
		{
			for (size_t c = columns; c < m_columns; c++) {
				for (size_t l = 0; l < m_lines; l++)
				{
                    at(l, c).~T();
					m_used--;
				}
			}
			m_columns = columns;
		}
		else if (m_order == Order::Column && m_lines == lines && columns * lines <= m_size)
		{
			for (size_t i = 0; i < m_lines; i++) {
				for (size_t k = m_columns; k < columns; k++)
				{
					new(&at(i, k)) T();
					m_used++;
				}
			}
			m_columns = columns;
		}
		else
		{
			MatrixBuffer_t tmp(lines, columns, m_order);
			copy(tmp, *this);
			*this = std::move(tmp);
		}
	}

	template< typename T >
	template< typename U >
	void MatrixBuffer_t<T>::dump(std::basic_ostream< U >& out) const
	{

		out << "size: " << m_lines << " x " << m_columns << '\n'
			//<< "order " << toString(m_order) << '\n'
			<< "Data:" << '\n';

		for (size_t y = 0; y < m_lines; y++)
		{
			out << "| ";
			for (size_t x = 0; x < m_columns; x++)
			{
				out << std::setw(6) << at(y, x) << ' ';
			}
			out << "|\n";
		}
	}


}//namespace matrix