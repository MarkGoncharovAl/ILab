/*************************************************************************
 *
 * Matrix.inl
 *
 * Created by dmitry
 * 22.02.2021
 *
 ***/

#include <algorithm>
#include <set>

namespace matrix
{

	template <typename T>
	T Matrix<T>::trace() const noexcept
	{
		const size_t lines = getLines();
		const size_t columns = getColumns();

		if (lines != columns || lines == 0) {
			return {};
		}
		
		T result = {};

		for (size_t i = 0; i < lines; i++)
		{
			result += at(i, i);
		}

		return result;
	}


	template <typename T>
	size_t Matrix<T>::rang() const
	{
	    if (m_actualRang) {
	        return m_rang;
	    }

	    Matrix tmp(*this);
	    tmp.gaussian();
	    return tmp.rang();
	}


	template <typename T>
    Matrix< T >& Matrix< T >::gaussian_from_(size_t bc /*= 0*/)&
    {
        const size_t mLines = getLines();
        const size_t mColumns = getColumns();
        size_t rang = 0;
        for (size_t cl = bc; cl < mColumns; cl++)
        {
            //find a*0 != 0
            bool found = false;
            for (size_t k = rang; k < mLines; k++)
            {
                if (std::abs(at(k, cl)) > EPSIL) {
                    if (k != rang) {
                        swopLines(rang, k);
                    }
                    found = true;
                    rang++;
                    break;
                }
            }
            if (!found)
                continue;

            auto& cur_elem = at(rang - 1, cl);
            for (size_t ln = rang; ln < mLines; ln++)
            {
                double multiplier = at(ln, cl) / cur_elem;
                for (size_t c = 0; c < mColumns && multiplier != T{}; c++)
                {
                    at(ln, c) -= at(rang - 1, c) * multiplier;
                }
            }
        }

        m_actualRang = true;
        m_rang = rang;

        return *this;
    }

    template <typename T>
    Matrix< T >& Matrix< T >::reversGaussian_from_(size_t bc /*= 0*/)&
    {
        const size_t mLines = getLines();
        const size_t mColumns = getColumns();
        size_t rang = 0;
        for (size_t cl = bc; cl < mColumns; cl++)
        {
            //find a*0 != 0
            bool found = false;
            for (size_t k = rang; k < mLines; k++)
            {
                if (std::abs(at(mLines - 1 - k, mColumns - 1 - cl)) > EPSIL) {
                    if (mLines - 1 - k != mLines - 1 - rang) {
                        swopLines(mLines- 1 - rang, mLines - 1 - k);
                    }
                    found = true;
                    rang++;
                    break;
                }
            }
            if (!found)
                continue;

            auto& cur_elem = at(mLines - rang, mColumns - 1 - cl);
            for (size_t ln = rang; ln < mLines; ln++)
            {
                double multiplier = at(mLines - 1 - ln, mColumns - 1 - cl) / cur_elem;
                for (size_t c = 0; c < mColumns && multiplier != T{}; c++)
                {
                    at(mLines - 1 - ln, c) -= at(mLines - rang, c) * multiplier;
                }
            }
        }

        m_actualRang = true;
        m_rang = rang;

        return *this;
    }

    /*
     * Reduces the matrix to a form in which the basis columns have only one non-zero element.
     */
    template <typename T>
    Matrix< T >& Matrix< T >::doubleGaussian_()&
    {
        gaussian_from_(0);
        const size_t nClm = getColumns();

        size_t c = 0;
        bool found = false;
        for (size_t l = getLines(); l != 0; l--) {
            for (c = 0; c < nClm; c++) {
                if (std::abs(at(l - 1, c) - T{}) > EPSIL) {
                    found = true;
                    break;
                }
            }
            if (found) break;
        }
        return reversGaussian_from_(nClm - c - 1);
    }

    template <typename T>
    std::vector< std::optional< size_t > > Matrix<T>::basicLinesAfterDG_() const
    {
        const size_t nClm = getColumns();
        const size_t nLines = getLines();

        std::vector< std::optional< size_t > > res(nClm);
        for (size_t l = 0, bc = 0; l < nLines; l++) {
            for (size_t c = bc; c < nClm; c++) {
                if (std::abs(at(l, c) - T{}) > EPSIL)
                {
                    res[c] = l;
                    bc = c + 1;
                    break;
                }
            }
        }

        return res;
    }


    template <typename T>
    Matrix< T > Matrix< T >::homogeneousSolve() const
    {
        return solve(std::vector(getLines(), T{})).second;
    }


    template< typename T >
    std::pair< Matrix< T >, Matrix< T>> Matrix< T >::solve(const std::vector< T > &freeMembers) const
    {
        if (getLines() != freeMembers.size()) {
            throw std::invalid_argument("");
        }

        Matrix< T > tmp(getLines(), getColumns() + 1);
        Matrix<T>::copy(tmp, *this);

        const size_t rang = tmp.rang();
        const size_t mLines = getLines();
        const size_t nClm = getColumns();

        for(size_t l = 0; l < mLines; l++) {
            tmp.at(l, nClm) = freeMembers[l];
        }

        tmp.doubleGaussian_();

        if (rang == 0 || tmp.rang() != rang) {
            return {};
        }

        const size_t dim_solution = getColumns() - rang;
        const std::vector< std::optional< size_t > > is_basicClm = tmp.basicLinesAfterDG_();
        const bool haveGeneralSolve = dim_solution > 0;
        const size_t num_iterations = (dim_solution == 0) ? 1 : dim_solution;

        Matrix< T > generalSolution(nClm, num_iterations);
        Matrix< T > particularSolution(nClm, num_iterations);
        for (size_t c = 0; c < num_iterations; c++)
        {
            size_t nonbasic_clm = 0;
            bool haveNonBasic = false;
            for (size_t l = 0, num = 0; l < nClm; l++) { //find nonbasic column with number 'c'
                if (!is_basicClm[l].has_value()) {
                    if (num == c) {
                        nonbasic_clm = l;
                        haveNonBasic = true;
                    }
                    num++;
                }
            }
            //assert(dim_solution > 1 && haveNonBasic || dim_solution == 1);

            for (size_t l = 0; l < nClm; l++)
            {
                if (is_basicClm[l].has_value())
                {
                    const size_t line_in_basic = is_basicClm[l].value();
                    if (haveNonBasic) {
                        generalSolution.at(l, c) = -tmp.at(line_in_basic, nonbasic_clm) / tmp.at(line_in_basic, l);
                        particularSolution.at(l, c) = (tmp.at(line_in_basic, nClm) - tmp.at(line_in_basic, nonbasic_clm)) / tmp.at(line_in_basic, l);
                    }
                    else {
                        if (haveGeneralSolve)
                            generalSolution.at(l, c) = 0;
                        particularSolution.at(l, c) = tmp.at(line_in_basic, nClm) / tmp.at(line_in_basic, l);
                    }
                }
                else
                {
                    if (haveNonBasic && l == nonbasic_clm)
                    {
                        generalSolution.at(l, c) = particularSolution.at(l, c) = 1;
                    }
                    else
                    {
                        generalSolution.at(l, c) = particularSolution.at(l, c) = 0;
                    }
                }
            }
        }

        return {particularSolution, generalSolution};
    }


    template <typename T >
	T Matrix<T>::determinanteSloww() const
	{
		T result = T();

		const size_t lines = getLines();
		const size_t columns = getColumns();

		if (lines == columns)
		{
			if (lines == 1) {
				return at(0, 0);
			}
			else if (lines == 2)
			{
				result = at(0, 0) * at(1, 1) - at(1, 0) * at(0, 1);
			}
			else
			{
				for (size_t k = 0; k < lines; k++)
				{
					result += at(0, k) * (submatrix(0, k).determinanteSloww()) * ((k % 2) ? -1 : 1);
				}
			}
		}

		return result;
	}


	template <typename T>
	double Matrix<T>::determinante() const
	{
		if (getLines() != getColumns() || getLines() == 0) {
			return {};
		}

		Matrix<double> tr_mtr{*this};

		const size_t N = getLines();
		bool minus = false;
		for (size_t cl = 0; cl < N; cl++)
		{
			//find a*0 != 0
			for (size_t k = cl; k < N; k++)
			{
				if (std::abs(tr_mtr.at(k, cl)) > EPSIL) {
					if (k != cl) {
						tr_mtr.swopLines(cl, k);
						minus = !minus;
					}
					break;
				}
			}
			//det = 0;
			if (std::abs(tr_mtr.at(cl, cl)) < EPSIL) {
				return 0.0;
			}

			auto& cur_elem = tr_mtr.at(cl, cl);
			for (size_t ln = cl + 1; ln < N; ln++)
			{
				double multiplier = tr_mtr.at(ln, cl) / cur_elem;
				for (size_t c = 0; c < N; c++)
				{
					tr_mtr.at(ln, c) -= tr_mtr.at(cl, c) * multiplier;
				}
			}
		}

		T result = tr_mtr.at(0, 0);
		for (size_t i = 1; i < N; i++) {
			result *= tr_mtr.at(i, i);
		}
		return result * ((minus) ? -1 : 1);
	}


	template <typename T>
	void Matrix<T>::swopLines(size_t l1_, size_t l2_)
	{
		const size_t columns = getColumns();
		for (size_t k = 0; k < columns; k++) {
			std::swap(at(l1_, k), at(l2_, k));
		}
	}


	template <typename T>
	void Matrix<T>::swopColumns(size_t cl1_, size_t cl2_)
	{
		const size_t lines = getLines();
		for (size_t k = 0; k < lines; k++) {
			std::swap(at(k, cl1_), at(k, cl2_));
		}
	}

	template <typename T>
	Matrix<T> Matrix<T>::submatrix(size_t deleted_line, size_t deleted_column) const
	{
		const size_t lines = getLines();
		const size_t columns = getColumns();

		if (columns == 0 || lines == 0) {
			return {};
		}

		Matrix<T> result(columns - 1, lines - 1, getOrder());

		for (size_t i = 0, j = 0; i < lines; i++)
		{
			if (i == deleted_line) {
				continue;
			}
			for (size_t k = 0, h = 0; k < columns; k++)
			{
				if (k == deleted_column) {
					continue;
				}
				result.at(j, h) = at(i, k);
				h++;
			}
			j++;
		}

		return result;
	}

	template <typename T>
	Matrix<T>& Matrix<T>::negate()&
	{
	    std::for_each(begin(), end(), [](T& elem)
        {
            elem = (-elem);
            return true;
        });

		return *this;
	}


	template <typename T>
	Matrix<T>& Matrix<T>::add(const Matrix& rhs_)&
	{
		Matrix<T> tmp(getLines(), getColumns(), getOrder());

		std::for_each(tmp.begin(), tmp.end(), [&rhs_, &lhs = *this](Elem<T> elem)
			{
				elem.val = rhs_.at(elem.line, elem.column) + lhs.at(elem.line, elem.line);
			});

		///

		return *this = std::move(tmp);
	}


	template <typename T>
	Matrix<T>& Matrix<T>::sub(const Matrix& rhs_)&
	{
		Matrix<T> tmp(getLines(), getColumns(), getOrder());

		std::for_each(tmp.begin(), tmp.end(), [&rhs_, &lhs = *this](Elem<T> elem)
		{
			elem.val = lhs.at(elem.line, elem.column) - rhs_.at(elem.line, elem.column);
		});

		///

		return *this = std::move(tmp);
	}


	template <typename T>
	Matrix<T>& Matrix<T>::mul(const T& num_)&
	{
		Matrix<T> tmp(getLines(), getColumns(), getOrder());

		std::for_each(begin(), end(), [&num_, &lhs = *this](Elem<T> elem)
		{
			elem.val = lhs.at(elem.line, elem.column) * num_;
			return true;
		});

		///

		return *this = std::move(tmp);
	}


	template <typename T>
    Matrix<T>& Matrix<T>::multiplication(const Matrix& that_)&
    {
		const size_t lines = getLines();
		const size_t columns = getColumns();

	    if (columns != that_.getLines())
        {
	        throw std::logic_error("matrix sizes are not valid for their composition");
        }

	    Matrix<T> result(lines, that_.getColumns());

	    for (size_t l = 0, ml = lines; l < ml; l++)
        {
	        for (size_t c = 0, mc = that_.getColumns(); c < mc; c++)
            {
	            auto& cur_res = result.at(l, c);
	            for (size_t N = 0; N < columns; N++)
                {
	                cur_res += at(l, N) * that_.at(N, c);
                }
            }
        }

		///

	    return *this = std::move(result);
    }

	template <typename T>
    template <typename U>
    bool Matrix<T>::operator != (const Matrix<U>& that_) const {
		return !equal(that_);
	}

	template <typename T>
	T& Matrix<T>::operator () (size_t _y, size_t _x)&
	{
		return const_cast<T&>(static_cast<const Matrix<T>*>(this)->operator()(_y, _x));
	}
	template <typename T>
	const T& Matrix<T>::operator () (size_t _y, size_t _x) const&
	{
		return at(_y, _x);
	}

	template <typename T>
	Matrix<T>& Matrix<T>::operator += (const Matrix& that_)& {
		return add(that_);
	}

	template <typename T>
	Matrix<T>& Matrix<T>::operator -= (const Matrix<T>& that_)& {
		return sub(that_);
	}

	template <typename T>
	Matrix<T>& Matrix<T>::operator *= (const T& num_)& {
		return mul(num_);
	}


	template <typename T>
	Matrix<T> operator + (const Matrix<T>& lhs_, const Matrix<T>& rhs_)
	{
		Matrix<T> res(lhs_);
		res += rhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator + (const Matrix<T>& lhs_, Matrix<T>&& rhs_)
	{
		Matrix<T> res(std::move(rhs_));
		res += lhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator + (Matrix<T>&& lhs_, const Matrix<T>& rhs_)
	{
		Matrix<T> res(std::move(lhs_));
		res += rhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator + (Matrix<T>&& lhs_, Matrix<T>&& rhs_)
	{
		Matrix<T> res(std::move(lhs_));
		res += rhs_;
		return res;
	}



	template <typename T>
	Matrix<T> operator - (const Matrix<T>& lhs_, const Matrix<T>& rhs_)
	{
		Matrix<T> res(lhs_);
		res -= rhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator - (const Matrix<T>& lhs_, Matrix<T>&& rhs_)
	{
		Matrix<T> res(std::move(rhs_));
		res -= lhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator - (Matrix<T>&& lhs_, const Matrix<T>& rhs_)
	{
		Matrix<T> res(std::move(lhs_));
		res -= rhs_;
		return res;
	}
	template <typename T>
	Matrix<T> operator - (Matrix<T>&& lhs_, Matrix<T>&& rhs_)
	{
		Matrix<T> res(std::move(lhs_));
		res -= rhs_;
		return res;
	}


	template <typename T>
	Matrix<T> operator * (const Matrix<T>& mtr_, const T& num_)
	{
		Matrix<T> res(mtr_);
		res.mul(num_);
		return res;
	}
	template <typename T>
	Matrix<T> operator * (Matrix<T>&& mtr_, const T& num_)
	{
		Matrix<T> res(std::move(mtr_));
		res.mul(num_);
		return res;
	}
	template <typename T>
	Matrix<T> operator * (const T& num_, const Matrix<T>& mtr_)
	{
		Matrix<T> res(mtr_);
		res.mul(num_);
		return res;
	}
	template <typename T>
	Matrix<T> operator * (const T& num_, Matrix<T>&& mtr_)
	{
		Matrix<T> res(std::move(mtr_));
		res.mul(num_);
		return res;
	}

    template <typename T>
    /*static*/ Matrix<T> Matrix<T>::identity(size_t size_)
    {
	    Matrix result(size_, size_);

	    for (size_t k = 0; k < size_; k++)
        {
	        result.at(k, k)++;
        }

	    return result;
    }
}//namespace matrix