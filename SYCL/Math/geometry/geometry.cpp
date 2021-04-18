#include "geometry.hpp"

namespace Msycl
{

    Dirichle::Dirichle ()
        : MySycl ()
    {}

    class LAMBDA_NAME1;
    Vector Dirichle::Mult (Msycl::Matrix<float>& matr , Vector& vect)
    {
        size_t global_size = vect.size ();
        if (matr.inner_size () != vect.size () + 1)
        {
            LOG_warning << "Can't multiply not equal sizes of matrix and vector!";
            return std::vector<float>{};
        }

        auto&& buffers = matr.GetBuffers ();
        auto&& vec_buf = CreateLinBuf (vect);

        Vector result (vect.size () , 0);
        auto&& result_buffer = CreateLinBuf (result);

        for (auto&& buffer : buffers)
        {
            submit ([&](cls::handler& cgh) {
                auto&& buffer_a = buffer.get_access<cls::access_mode::read> (cgh);
                auto&& vec_buf_a = vec_buf.get_access<cls::access_mode::read> (cgh);
                auto&& result_buffer_a = result_buffer.get_access<cls::access_mode::read_write> (cgh);

                cgh.parallel_for<class LAMBDA_NAME1> (cls::range<1>{global_size} , [=](cls::id<1> id) {
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

    class LAMBDA_NAME2;
    Vector Dirichle::Mult (float num , Vector const& vector)
    {
        Vector vect (vector);
        size_t global_size = vect.size ();
        auto&& vec_buf = CreateLinBuf (vect);

        submit ([&](cls::handler& cgh) {
            auto&& buffer_a = vec_buf.get_access<cls::access_mode::read_write> (cgh);

            cgh.parallel_for<class LAMBDA_NAME2> (cls::range<1>{global_size} , [=](cls::id<1> id) {
                const int i = id[0];
                buffer_a[i] *= num;
            });
        });

        return vect;
    }
    // Vector Dirichle::Sub (Vector const& lhs , Vector& rhs)
    // {
    //     Vector vect (lhs);
    //     size_t global_size = vect.size ();
    //     auto&& vec_buf = CreateBuffer (vect);
    //     auto&& rhs_buf = CreateBuffer (rhs);

    //     cl::Kernel kernel (program_ , "Sub");
    //     kernel.setArg (0 , vec_buf);
    //     kernel.setArg (1 , rhs);
    //     RunEvent (kernel , global_size , 1);

    //     cl::copy (queue_ , vec_buf , vect.begin () , vect.end ());
    //     return vect;
    // }
    // Vector Dirichle::Sum (Vector const& lhs , Vector& rhs)
    // {
    //     Vector vect (lhs);
    //     size_t global_size = vect.size ();
    //     auto&& vec_buf = CreateBuffer (vect);
    //     auto&& rhs_buf = CreateBuffer (rhs);

    //     cl::Kernel kernel (program_ , "Sum");
    //     kernel.setArg (0 , vec_buf);
    //     kernel.setArg (1 , rhs);
    //     RunEvent (kernel , global_size , 1);

    //     cl::copy (queue_ , vec_buf , vect.begin () , vect.end ());
    //     return vect;

    // }

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
            // LOG_trace << "Matrix: " << matr;
            // LOG_trace << "r: " << r;
            // LOG_trace << "z: " << z;
            // LOG_trace << "x: " << x;
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

    Vector::Vector (size_t size)
        : data_ (size)
    {}
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

    float Vector::norm () const noexcept
    {
        float out = 0;
        for (auto&& elem : data_)
            out += elem * elem;
        return out;
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