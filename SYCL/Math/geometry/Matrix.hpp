#pragma once

#include "../MySycl/MySycl.hpp"
#include <Logging.hpp>
#include "../matrix/Matrix.h"
#include <fstream>

namespace Msycl
{
    template <typename T>
    class Matrix
    {
    public:
        Matrix (matrix::Matrix<T> const& matr);

        /*dangerous, BUT
        1) data_ and buffers_ will live the same time
        2) we often use GetBuffer for multiplication
        3) copying is useless, because we have pointers on data_
        -> shouldn't be copied
        */
        std::vector<cls::buffer<T , 1>>& GetBuffers () { return buffers_; }

        size_t inner_size () const noexcept { return data_.front ().size (); }
        size_t outer_size () const noexcept { return data_.size (); }

        template <typename stream_t>
        void dump (stream_t& stream) const;

    private:
        //vector has data in diagonal's view:
        //therefore sparse matrix are multiplying fast
        std::vector<std::vector<T>> data_;

        //Creating buffers in queue that is in syck
        std::vector<cls::buffer<T , 1>> buffers_;
    };

    namespace Helper
    {
        //Checking for not zero element in matrix
        //return false if every element is zero
        template <typename T>
        bool CheckUpDiag (matrix::Matrix<T> const& matr , size_t num);
        template <typename T>
        bool CheckDownDiag (matrix::Matrix<T> const& matr , size_t num);
    }

    ///////////////////////////////////////////
    ///////////////////////////////////////////
    ///////////////////////////////////////////
    ///////////////////////////////////////////
    //Realizations

    template <typename T>
    Matrix<T>::Matrix (matrix::Matrix<T> const& matr)
    {
        if (matr.empty ())
        {
            LOG_warning << "Can't create Matrix from empty data";
            return;
        }
        if (matr.getLines () != matr.getColumns ())
        {
            LOG_warning << "Can't create clM::Matrix from not diagonal";
            return;
        }

        LOG_trace << "Creating clM::Matrix";
        size_t matr_size = matr.getLines ();

        //going right in matrix diagonals
        for (size_t i = 0; i < matr_size; ++i)
        {
            //checking for not-diag matrix
            if (!Helper::CheckUpDiag (matr , i)) //if everything zero
                continue;

            std::vector<T> adding;
            adding.reserve (matr_size + 1);
            adding.push_back (-1 * static_cast<float>(i)); //shifts information for kernel

            for (size_t j = 0; j < i; ++j)
                adding.push_back (0); //shifting

            for (size_t j = i; j < matr_size; ++j)
                adding.push_back (matr (j - i , j)); //copying diag

            data_.push_back (std::move (adding));
        }

        //going down in matrix diagonals
        for (size_t i = 1; i < matr_size; ++i)
        {
            //checking for not-diag matrix
            if (!Helper::CheckDownDiag (matr , i)) //if everything zero
                continue;

            std::vector<T> adding;
            adding.reserve (matr_size + 1);
            adding.push_back (static_cast<float>(i)); //shifts information for kernel

            for (size_t j = i; j < matr_size; ++j)
                adding.push_back (matr (j , j - i)); //copying diag

            for (size_t j = 0; j < i; ++j)
                adding.push_back (0); //shifting

            data_.push_back (std::move (adding));
        }

        buffers_.reserve (data_.size ());

        for (auto&& container : data_)
            buffers_.push_back (std::move (CreateLinBuf (container)));
    }

    template <typename T>
    template <typename stream_t>
    void Matrix<T>::
        dump (stream_t& stream) const
    {
        stream << "Dumping clM::Matrix:\n";
        for (size_t i = 0; i < data_.size (); ++i)
        {
            stream << "[" << i << "] [shift:";
            stream << data_[i].front () << "] ";
            for (auto&& iter = data_[i].begin () + 1; iter != data_[i].end (); ++iter)
                stream << *iter << " ";
            stream << "\n";
        }
        stream << "Dump has ended\n";
    }

    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    ////////////////////////////////////////////////
    //Helper

    template <typename T>
    bool Helper::CheckUpDiag (matrix::Matrix<T> const& matr , size_t num)
    {
        for (size_t j = num; j < matr.getLines (); ++j)
            if (matr (j - num , j) != 0)
                return true;
        return false;
    }
    template <typename T>
    bool Helper::CheckDownDiag (matrix::Matrix<T> const& matr , size_t num)
    {
        for (size_t j = num; j < matr.getLines (); ++j)
            if (matr (j , j - num) != 0)
                return true;
        return false;
    }

}


////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
//Extra functions

template <typename T>
std::ostream& operator<<(std::ostream& stream , Msycl::Matrix<T> const& matr)
{
    matr.dump (stream);
    return stream;
}

template <typename T>
std::ofstream& operator<<(std::ofstream& stream , Msycl::Matrix<T> const& matr)
{
    matr.dump (stream);
    return stream;
}