#pragma once
#include "Matrix.hpp"

namespace Msycl
{
    class Vector
    {
    public:
        using value_type = float;

        Vector (size_t size);
        Vector (size_t num , float default_val);
        Vector (std::vector<float>&& data);
        Vector (const Vector& that) = default;
        Vector (Vector&& that) = default;
        Vector& operator=(const Vector& that) = default;
        Vector& operator=(Vector&& that) = default;

        void multiply (float num);
        float norm () const noexcept;

        //in order to work with OpenCL::CreateBuffer
        //////////////
        size_t size () const noexcept { return data_.size (); }
        float* data () noexcept { return data_.data (); }
        float front () const noexcept { return data_.front (); }
        //////////////

        //this operators are used in CPU
        //but in the Dirichle you can do it on GPU
        Vector operator + (Vector& that) const&;
        Vector operator + (Vector& that)&&;
        Vector operator + (Vector&& that) const;
        Vector operator - (Vector& that) const&;
        Vector operator - (Vector& that)&&;
        Vector operator - (Vector&& that) const;

        void operator += (Vector const& that);
        void operator -= (Vector const& that);

        float operator * (Vector const& that);

        float& operator[](size_t num);
        float operator[](size_t num) const;

        auto begin () noexcept { return data_.begin (); }
        auto end () noexcept { return data_.end (); }

        template <typename stream_t>
        void dump (stream_t& stream) const;

    private:
        std::vector<float> data_;
    };

    class Dirichle : private MySycl
    {
    public:

        Dirichle ();

        //GPU power - but it's slower than CPU except matrix * vector...
        //Therefore I'm using only Mult in both cases
        ////////////////////////////
        Vector Mult (Matrix<float>& matr , Vector& vect);
        Vector Mult (float num , Vector const& vect);

        /*
        vector * vector can't be optimized, because:
        1) multi-threading on 1 element (barier is helpless)
        2) the main time is getting elements from memory
        */

        /*
        we can't do rhs 'const' in order to get 'data()'
        during creating a buffer
        */
        // Vector Sub (Vector const& lhs , Vector& rhs); //rhs = const
        // Vector Sum (Vector const& lhs , Vector& rhs); //rhs = const
        ////////////////////////////

        //Main function in algorithm
        Vector Solve (Matrix<float>& matr , Vector& vect);

    private:

        //Main function in algorithm
        void SolveIteration (Matrix<float>& A , Vector& r , Vector& z , Vector& x);

    };
}

Msycl::Vector operator * (float num , const Msycl::Vector& vector);
Msycl::Vector operator * (const Msycl::Vector& vector , float num);
std::ostream& operator << (std::ostream& stream , Msycl::Vector const& vector);


namespace Msycl
{
    template <typename stream_t>
    void Vector::dump (stream_t& stream) const
    {
        for (size_t i = 0; i < data_.size (); ++i)
            stream << data_[i] << " ";
    }
}