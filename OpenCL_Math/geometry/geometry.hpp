#pragma once
#include "Matrix.hpp"

namespace clM
{
    class Vector
    {
    public:
        Vector (size_t num , float default_val);
        Vector (std::vector<float>&& data);
        Vector (Vector& that) = default;
        Vector (Vector&& that) = default;

        void multiply (float num);

        //in order to work with OpenCL::CreateBuffer
        //////////////
        size_t size () const noexcept { return data_.size (); }
        float* data () noexcept { return data_.data (); }
        float front () const noexcept { return data_.front (); }
        //////////////

        clM::Vector operator + (Vector& that) const&;
        clM::Vector operator + (Vector& that)&&;
        clM::Vector operator + (Vector&& that) const;
        clM::Vector operator - (Vector& that) const&;
        clM::Vector operator - (Vector& that)&&;
        clM::Vector operator - (Vector&& that) const;

        void operator += (Vector const& that);
        void operator -= (Vector const& that);

        float operator * (Vector const& that);

        float& operator[](size_t num);
        float operator[](size_t num) const;

        auto begin () noexcept { return data_.begin (); }
        auto end () noexcept { return data_.end (); }

    private:
        std::vector<float> data_;
    };

    class Dirichle : public OpenCL
    {
    public:

        Dirichle (int argc , char* argv []);
        Dirichle ();
        Vector Multiply (clM::Matrix<float>& matr , Vector& vect);

    private:
        void RunEvent (cl::Kernel& kernel , size_t global_size , size_t local_size);
    };
}

clM::Vector& operator * (float num , clM::Vector& vector);
clM::Vector& operator * (clM::Vector& vector , float num);