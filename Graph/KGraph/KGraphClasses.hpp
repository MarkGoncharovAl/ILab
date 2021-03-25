#pragma once

#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "../Common_libs/Errors/Errors.hpp"
#include "../Common_libs/Color.hpp"

namespace MCGraph
{
    //I use it, because it's quite convenient.
    //although not efficient in using memory...
    class NamesEq_t final
    {
        public:
        //  returns true if element was inserted
        bool AddInfo_NTO (int new_node , int old_node);
        int GetInfo_NTO (int node) const;

        bool AddInfo_OTN (int old_node , int new_node);
        int GetInfo_OTN (int node) const;

        private:
        std::unordered_map<int , int> new_TO_old_;
        std::unordered_map<int , int> old_TO_new_;
    };

    //in order to colorize nodes and define bipartite
    struct Color final
    {
        //USING
        //--------------------------------------------
        using type = char;

        static constexpr type START_VALUE = -1;
        static constexpr type BLUE = 0;
        static constexpr type RED = 1;
        //--------------------------------------------

        explicit operator bool () const noexcept { return data_ != START_VALUE; }
        bool empty () const noexcept { return data_ == START_VALUE; }

        bool is_blue () const noexcept { return data_ == BLUE; }

        //true if color was changed
        bool change_color () noexcept;

        //out char of data_ to print in std::cout ('b' or 'r')
        char print () const noexcept;

        //returns differ color 
        char get_another () const noexcept;

        bool operator == (const Color& that) const noexcept { return data_ == that.data_; }

        type data_ = START_VALUE;
    };

    template <typename ET>
    struct Edge final
    {
        static constexpr int NOT_FILLED = -1;

        int node_ = NOT_FILLED;
        int next_ = NOT_FILLED;
        int prev_ = NOT_FILLED;
        ET data_;

    };

    template<typename NT>
    struct Node final
    {
        static constexpr int NOT_FILLED = -1;

        int next_ = NOT_FILLED;
        int prev_ = NOT_FILLED;
        NT data_;
        Color color_;
    };
}