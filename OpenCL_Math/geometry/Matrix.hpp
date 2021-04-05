#pragma once

#include "../MC_OpenCL/MC_OpenCL.hpp"
#include "../matrix/Matrix.h"
#include <fstream>

namespace clM
{
    template <typename T>
    class Matrix
    {
        public:
        Matrix (matrix::Matrix<T> const& matr);

        std::vector<cl::Buffer> GetBuffers (OpenCL* opencl);

        size_t inner_size () const noexcept { return data_.front ().size (); }
        size_t outer_size () const noexcept { return data_.size (); }

        template <typename stream_t>
        void dump (stream_t& stream) const;
        
        private:
        std::vector<std::vector<T>> data_;
    };

    struct Helper
    {
        template <typename T>
        static bool CheckUpDiag (matrix::Matrix<T> const& matr , size_t num);
        template <typename T>
        static bool CheckDownDiag (matrix::Matrix<T> const& matr , size_t num);
    };

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
            adding.reserve (matr_size);

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
            adding.reserve (matr_size);

            for (size_t j = i; j < matr_size; ++j)
                adding.push_back (matr (j , j - i)); //copying diag

            for (size_t j = 0; j < i; ++j)
                adding.push_back (0); //shifting

            data_.push_back (std::move (adding));
        }
    }

    template <typename T>
    template <typename stream_t>
    void Matrix<T>::
        dump (stream_t& stream) const
    {
        stream << "Dumping clM::Matrix:\n";
        for (size_t i = 0; i < data_.size (); ++i)
        {
            stream << "[" << i << "] ";
            for (auto&& elem : data_[i])
                stream << elem << " ";
            stream << "\n";
        }
        stream << "Dump has ended\n";
    }

    template <typename T>
    std::vector<cl::Buffer> Matrix<T>::
        GetBuffers (OpenCL* cl)
    {
        std::vector<cl::Buffer> buffers;
        buffers.reserve (data_.size ());

        for (auto&& container : data_)
            buffers.push_back (std::move (cl->CreateBuffer (container)));

        return buffers;
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
std::ostream& operator<<(std::ostream& stream , clM::Matrix<T> const& matr)
{
    matr.dump (stream);
    return stream;
}

template <typename T>
std::ofstream& operator<<(std::ofstream& stream , clM::Matrix<T> const& matr)
{
    matr.dump (stream);
    return stream;
}