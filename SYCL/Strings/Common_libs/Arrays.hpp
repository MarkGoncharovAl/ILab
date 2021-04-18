#pragma once
#include <iostream>
#include <functional>
#include <vector>
#include <array>
#include "Errors.hpp"

namespace MLib
{
    //Array have to get size()
    //operator bool
    //operator []

    template <typename Elem_t , size_t dim1 , size_t dim2>
    void DoEveryElem
    (std::array<Elem_t , dim1>& lhs , std::array<Elem_t , dim2>& rhs , std::function<void (Elem_t& , Elem_t&)> func)
    {
        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (dim1 , dim2);
            for (decltype(min_size) i = 0; i < min_size; ++i)
                func (lhs[i] , rhs[i]);
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }

    template <typename Elem_t>
    void DoEveryElem
    (std::vector<Elem_t>& lhs , std::vector<Elem_t>& rhs , std::function<void (Elem_t& , Elem_t&)> func)
    {
        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (lhs.size () , rhs.size ());
            for (decltype(min_size) i = 0; i < min_size; ++i)
                func (lhs[i] , rhs[i]);
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }

    template <typename Elem_t , size_t dim1 , size_t dim2>
    void DoEveryElem
    (std::array<Elem_t , dim1>& lhs , const std::array<Elem_t , dim2>& rhs , std::function<void (Elem_t& , const Elem_t&)> func)
    {
        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (dim1 , dim2);
            for (decltype(min_size) i = 0; i < min_size; ++i)
                func (lhs[i] , rhs[i]);
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }

    template <typename Elem_t>
    void DoEveryElem
    (std::vector<Elem_t>& lhs , const std::vector<Elem_t>& rhs , std::function<void (Elem_t& , const Elem_t&)> func)
    {
        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (lhs.size () , rhs.size ());
            for (decltype(min_size) i = 0; i < min_size; ++i)
                func (lhs[i] , rhs[i]);
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }












    template <typename Elem_t , typename Out_t , size_t dim1 , size_t dim2>
    std::vector<Out_t> DoEveryElem
    (std::array<Elem_t , dim1>& lhs , std::array<Elem_t , dim2>& rhs , std::function<void (Elem_t& , Elem_t&)> func)
    {
        std::vector<Out_t> output;

        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (dim1 , dim2);
            output.reserve (min_size);
            for (auto i = 0; i < min_size; ++i)
                output.push_back (std::move (func (lhs[i] , rhs[i])));
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");

        return output;
    }

    template <typename Elem_t, typename Out_t>
    std::vector<Out_t> DoEveryElem
    (std::vector<Elem_t>& lhs , std::vector<Elem_t>& rhs , std::function<void (Elem_t& , Elem_t&)> func)
    {
        std::vector<Out_t> output;

        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (lhs.size () , rhs.size ());
            output.reserve (min_size);
            for (decltype(min_size) i = 0; i < min_size; ++i)
                output.push_back (std::move (func (lhs[i] , rhs[i])));
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }

    template <typename Elem_t, typename Out_t , size_t dim1 , size_t dim2>
    std::vector<Out_t> DoEveryElem
    (std::array<Elem_t , dim1>& lhs , const std::array<Elem_t , dim2>& rhs , std::function<void (Elem_t& , const Elem_t&)> func)
    {
        std::vector<Out_t> output;

        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (dim1 , dim2);
            output.reserve (min_size);
            for (auto i = 0; i < min_size; ++i)
                output.push_back (std::move (func (lhs[i] , rhs[i])));
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");

        return output;
    }

    template <typename Elem_t, typename Out_t>
    std::vector<Out_t> DoEveryElem
    (std::vector<Elem_t>& lhs , const std::vector<Elem_t>& rhs , std::function<void (Elem_t& , const Elem_t&)> func)
    {
        std::vector<Out_t> output;

        if (!lhs.empty () && !rhs.empty ())
        {
            auto min_size = std::min (lhs.size () , rhs.size ());
            output.reserve (min_size);
            for (decltype(min_size) i = 0; i < min_size; ++i)
                output.push_back (std::move (func (lhs[i] , rhs[i])));
        }
        else //lhs or rhs is empty
            WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    }

    // template <typename Array_t , typename Elem_t , typename Out_t>
    // std::vector<Out_t> DoEveryElem
    // (Array_t& lhs , Array_t& rhs , std::function<Out_t (Elem_t& , Elem_t&)> func)
    // {
        // std::vector<Out_t> output;

        // if (lhs && rhs)
        // {
        //     auto min_size = std::min (lhs.size () , rhs.size ());
        //     output.reserve (min_size);
        //     for (auto i = 0; i < min_size; ++i)
        //         output.push_back (func (lhs[i] , rhs[i]));
        // }
        // else //lhs or rhs is empty
        //     WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");

        // return output;
    // }

    // template <typename Array_t , typename Elem_t>
    // void DoEveryElem
    // (Array_t& lhs , Array_t& rhs , Array_t& frs, std::function<void (Elem_t& , Elem_t&, Elem_t&)> func)
    // {
    //     if (lhs && rhs && frs)
    //     {
    //         auto min_size = std::min (lhs.size () , rhs.size (), frs.size());
    //         for (auto i = 0; i < min_size; ++i)
    //             func (lhs[i] , rhs[i], frs[i]);
    //     }
    //     else //lhs or rhs is empty
    //         WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");
    // }

    // template <typename Array_t , typename Elem_t , typename Out_t>
    // std::vector<Out_t> DoEveryElem
    // (Array_t& lhs , Array_t& rhs , Array_t& frs, std::function<Out_t (Elem_t&, Elem_t& , Elem_t&)> func)
    // {
    //     std::vector<Out_t> output;

    //     if (lhs && rhs && frs)
    //     {
    //         auto min_size = std::min (lhs.size () , rhs.size (), frs.size());
    //         output.reserve (min_size);
    //         for (auto i = 0; i < min_size; ++i)
    //             output.push_back (func (lhs[i] , rhs[i], frs[i]));
    //     }
    //     else //lhs or rhs is empty
    //         WARNING ("DoEveryElem wasn't done anthing - one or more arrays are empty!");

    //     return output;
    // }
} // namespace MLib
