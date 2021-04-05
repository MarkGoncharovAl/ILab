#include "geometry.hpp"

namespace clM
{
    Dirichle::Dirichle (int argc , char* argv [])
        : OpenCL (argc , argv , "../geometry/dirichle.cl")
    {}
    Dirichle::Dirichle ()
        : OpenCL ("../geometry/dirichle.cl")
    {}

    Vector Dirichle::
        Multiply (clM::Matrix<float>& matr , Vector& vect)
    {
        size_t global_size = vect.size ();
        if (matr.inner_size () != global_size)
        {
            LOG_warning << "Can't multiply not equal sizes of matrix and vector!";
            return std::vector<float>{};
        }

        auto&& buffers = matr.GetBuffers (this);
        auto&& vec_buf = CreateBuffer (vect);

        Vector result (global_size , 0);
        auto&& result_buffer = CreateBuffer (result);

        cl::Kernel kernel (program_ , "Multiply");

        for (auto&& buffer : buffers)
        {
            kernel.setArg (0 , buffer);
            kernel.setArg (1 , vec_buf);
            kernel.setArg (2 , result_buffer);
            RunEvent (kernel , global_size , 1);
        }

        cl::copy (queue_ , result_buffer , result.begin () , result.end ());
        return result;
    }

    void Dirichle::
        RunEvent (cl::Kernel& kernel , size_t global_size , size_t local_size)
    {
        cl::Event event;
        queue_.enqueueNDRangeKernel (kernel , 0 , global_size , local_size , nullptr , &event);
        event.wait ();
    }


    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //Vector

    Vector::Vector (std::vector<float>&& data)
        : data_ (std::move (data))
    {}
    Vector::Vector (size_t num , float default_val)
        : data_ (num , default_val)
    {}

    void Vector::multiply (float num)
    {
        for (auto& elem : data_)
            elem *= num;
    }
    float& Vector::operator[](size_t num)
    {
        return data_[num];
    }
    float Vector::operator[](size_t num) const
    {
        return data_[num];
    }

    void Vector::operator += (Vector const& that)
    {
        if (data_.size () != that.size ())
        {
            LOG_warning << "can't sum not eqaul vectors";
            return;
        }
        for (size_t i = 0; i < data_.size (); ++i)
            data_[i] += that[i];
    }
    void Vector::operator -= (Vector const& that)
    {
        if (data_.size () != that.size ())
        {
            LOG_warning << "can't sum not eqaul vectors";
            return;
        }
        for (size_t i = 0; i < data_.size (); ++i)
            data_[i] -= that[i];
    }

    clM::Vector Vector::operator + (Vector& that) const&
    {
        Vector out (that);
        out += *this;
        return out;
    }
    clM::Vector Vector::operator + (Vector& that)&&
    {
        operator+=(that);
        return *this;
    }
    clM::Vector Vector::operator + (Vector&& that) const
    {
        that += *this;
        return that;
    }
    clM::Vector Vector::operator - (Vector& that) const&
    {
        Vector out (that);
        out -= *this;
        return out;
    }
    clM::Vector Vector::operator - (Vector& that)&&
    {
        operator-=(that);
        return *this;
    }
    clM::Vector Vector::operator - (Vector&& that) const
    {
        that += *this;
        return that;
    }

    float Vector::operator * (Vector const& that)
    {
        float out = 0;
        for (size_t i = 0; i < data_.size (); ++i)
            out += data_[i] * that[i];
        return out;
    }
}

clM::Vector& operator * (float num , clM::Vector& vector)
{
    vector.multiply (num);
    return vector;
}
clM::Vector& operator * (clM::Vector& vector , float num)
{
    vector.multiply (num);
    return vector;
}