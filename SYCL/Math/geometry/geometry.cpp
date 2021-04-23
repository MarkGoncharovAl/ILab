#include "geometry.hpp"

namespace Msycl
{
    class MatrixVector;
    Vector Dirichle::Mult (Msycl::Matrix<float>& matr , Vector& vect)
    {
        size_t global_size = vect.size ();
        if (matr.inner_size () != vect.size () + 1)
        {
            LOG_warning << "Can't multiply not equal sizes of matrix and vector!";
            return std::vector<float>{};
        }

        auto&& buffers = matr.GetBuffers ();
        auto&& vec_buf = CreateLinBuf (vect.begin(), vect.end());

        Vector result (vect.size () , 0);
        auto&& result_buffer = CreateLinBuf (result.begin(), result.end());

        for (auto&& buffer : buffers)
        {
            submit ([&](cls::handler& cgh) {
                auto&& buffer_a = buffer.get_access<cls::access_mode::read> (cgh);
                auto&& vec_buf_a = vec_buf.get_access<cls::access_mode::read> (cgh);
                auto&& result_buffer_a = result_buffer.get_access<cls::access_mode::read_write> (cgh);

                cgh.parallel_for<class MatrixVector> (cls::range<1>{global_size} , [=](cls::id<1> id) {
                    const int i = id[0];
                    const float cur_buf0 = buffer_a[0];
                    float cur_buffer = buffer_a[i + 1];
                    if (cur_buffer == 0)
                        return;

                    uint shift = (uint)((float)i + cur_buf0);
                    result_buffer_a[shift] += cur_buffer * vec_buf_a[i];
                });
            });
        }

        return result;
    }

    class MultVectorNum;
    Vector Dirichle::Mult (float num , Vector const& vector)
    {
        Vector vect (vector);
        size_t global_size = vect.size ();
        auto&& vec_buf = CreateLinBuf (vect.begin(), vect.end());

        submit ([&](cls::handler& cgh) {
            auto&& buffer_a = vec_buf.get_access<cls::access_mode::read_write> (cgh);

            cgh.parallel_for<class MultVectorNum> (cls::range<1>{global_size} , [=](cls::id<1> id) {
                const int i = id[0];
                buffer_a[i] *= num;
            });
        });

        return vect;
    }

    //MAIN FUNCTIONS IN ALGORITHM
    /////////////////////////////////////////
    Vector Dirichle::Solve (Matrix<float>& matr , Vector& vect)
    {
        //like in wikipedia (NVidia just stole this method): 
        //https://en.wikipedia.org/wiki/Conjugate_gradient_method

        /*
        than smaller -> then better to answer!
        it shows maximum norm of remaining to asnwer vector
        */
        constexpr float accuracy = 0.005;

        Vector x (vect.size () , 0);
        auto&& r = vect - Mult (matr , x);
        auto&& z = r;
        float norm_b = vect.norm ();

        while (r.norm () > accuracy)
        {
            LOG_trace << "Matrix: " << matr;
            LOG_trace << "r: " << r;
            LOG_trace << "z: " << z;
            LOG_trace << "x: " << x;
            SolveIteration (matr , r , z , x);
        }
        return x;
    }

    void Dirichle::SolveIteration (Matrix<float>& A , Vector& r , Vector& z , Vector& x)
    {
        auto alpha = (r * r) / (Mult (A , z) * z);
        LOG_trace << "alpha: " << alpha;
        x = x + Mult (alpha , z);
        LOG_trace << "x: " << x;
        auto new_r = r - Mult (alpha , Mult (A , z));
        LOG_trace << "new_r: " << new_r;
        auto betta = (r * r) / (new_r * new_r);
        LOG_trace << "betta: " << betta;
        z = new_r + Mult (betta , z);
        LOG_trace << "z: " << z;
        r = new_r;
    }

    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    //Vector

    void Vector::multiply (float num)
    {
        for (auto& elem : data_)
            elem *= num;
    }

    float Vector::norm () const noexcept
    {
        float out = 0;
        for (auto&& elem : data_)
            out += elem * elem;
        return out;
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

    Vector Vector::operator + (Vector& that) const&
    {
        Vector out (that);
        out += *this;
        return out;
    }
    Vector Vector::operator + (Vector& that)&&
    {
        operator+=(that);
        return *this;
    }
    Vector Vector::operator + (Vector&& that) const
    {
        that += *this;
        return that;
    }
    Vector Vector::operator - (Vector& that) const&
    {
        Vector out (*this);
        out -= that;
        return out;
    }
    Vector Vector::operator - (Vector& that)&&
    {
        operator-=(that);
        return *this;
    }
    Vector Vector::operator - (Vector&& that) const
    {
        that -= *this;
        that.multiply (-1);
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

Msycl::Vector operator * (float num , const Msycl::Vector& vector)
{
    Msycl::Vector out = vector;
    out.multiply (num);
    return out;
}
Msycl::Vector operator * (const Msycl::Vector& vector , float num)
{
    Msycl::Vector out = vector;
    out.multiply (num);
    return out;
}

std::ostream& operator << (std::ostream& stream , Msycl::Vector const& vector)
{
    vector.dump<std::ostream> (stream);
    return stream;
}