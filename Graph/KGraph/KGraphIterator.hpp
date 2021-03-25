#pragma once
#include "KGraphClasses.hpp"

namespace MCGraph
{
    template <typename ET>
    class iterator_edge final
    {
        public:
        static constexpr int NOT_FILLED = -1;

        private:
        int node_ = NOT_FILLED;
        Edge<ET>* edges_ = nullptr;

        public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t; //should be deleted
        using value_type = Edge<ET>;
        using reference = Edge<ET>&;
        using pointer = Edge<ET>*;

        iterator_edge (int node , Edge<ET>* edges) noexcept : node_ (node) , edges_ (edges) {}
        iterator_edge () = default;
        iterator_edge (const iterator_edge& that) = default;
        iterator_edge& operator=(const iterator_edge& that) = default;

        int GetPosition () const noexcept { return node_; }

        Edge<ET>& operator* () { return edges_[node_]; }
        Edge<ET>* operator->() { return edges_ + node_; }
        void operator++() { node_ = edges_[node_].next_; }
        iterator_edge<ET>& operator++(int) { node_ = edges_[node_].next_; return *this; }
        void operator--() { node_ = edges_[node_].prev_; }
        iterator_edge<ET>& operator--(int) { node_ = edges_[node_].prev_; return *this; }
        explicit operator bool () const { return node_ == NOT_FILLED; }
        bool operator == (const iterator_edge& that) const { return node_ == that.node_; }
        bool operator != (const iterator_edge& that) const { return node_ != that.node_; }
    };

    template <typename ET>
    class const_iterator_edge final
    {
        public:
        static constexpr int NOT_FILLED = -1;

        private:
        int node_ = NOT_FILLED;
        const Edge<ET>* edges_ = nullptr;

        public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = size_t; //should be deleted
        using value_type = const Edge<ET>;
        using reference = const Edge<ET>&;
        using pointer = const Edge<ET>*;

        const_iterator_edge (int node , const Edge<ET>* edges) noexcept : node_ (node) , edges_ (edges) {}
        const_iterator_edge () = default;
        const_iterator_edge (const const_iterator_edge& that) = default;
        const_iterator_edge& operator=(const const_iterator_edge& that) = default;

        int GetPosition () const noexcept { return node_; }

        const Edge<ET>& operator* () { return edges_[node_]; }
        const Edge<ET>* operator->() { return edges_ + node_; }
        void operator++() { node_ = edges_[node_].next_; }
        const_iterator_edge<ET>& operator++(int) { node_ = edges_[node_].next_; return *this; }
        void operator--() { node_ = edges_[node_].prev_; }
        const_iterator_edge<ET>& operator--(int) { node_ = edges_[node_].prev_; return *this; }
        explicit operator bool () const { return node_ == NOT_FILLED; }
        bool operator == (const const_iterator_edge& that) const { return node_ == that.node_; }
        bool operator != (const const_iterator_edge& that) const { return node_ != that.node_; }
    };

    template <typename ET , typename NT>
    class iterator final
    {
        public:
        static constexpr int NOT_FILLED = -1;

        private:
        int node_ = NOT_FILLED;
        const NamesEq_t* names_ = nullptr;
        Edge<ET>* edges_ = nullptr;
        Node<NT>* nodes_ = nullptr;

        public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = int;
        using value_type = int;
        using reference = int&;
        using pointer = int*;

        iterator (int node , const NamesEq_t* names , Edge<ET>* edges , Node<NT>* nodes) noexcept
            : node_ (node) ,
            names_ (names) ,
            edges_ (edges) ,
            nodes_ (nodes)
        {}
        iterator () = default;
        iterator (const iterator& that) = default;
        iterator& operator=(const iterator& that) = default;

        int operator* () { return names_->GetInfo_NTO (node_); }
        void operator++() { ++node_; }
        iterator& operator++(int) { ++node_; return *this; }
        void operator--() { --node_; }
        iterator& operator--(int) { --node_; return *this; }
        explicit operator bool () const { return node_ == NOT_FILLED; }
        bool operator == (const iterator& that) const { return node_ == that.node_; }
        bool operator != (const iterator& that) const { return node_ != that.node_; }

        iterator_edge<ET> begin () { return iterator_edge<ET> (nodes_[node_].next_ , edges_); }
        iterator_edge<ET> end () { return iterator_edge<ET> (NOT_FILLED , nullptr); }
    };

    template <typename ET , typename NT>
    class const_iterator final
    {
        public:
        static constexpr int NOT_FILLED = -1;

        private:
        int node_ = NOT_FILLED;
        const NamesEq_t* names_ = nullptr;
        const Edge<ET>* edges_ = nullptr;
        const Node<NT>* nodes_ = nullptr;

        public:
        using iterator_category = std::bidirectional_iterator_tag;
        using difference_type = int;
        using value_type = int;
        using reference = int&;
        using pointer = int*;

        const_iterator (int node , const NamesEq_t* names , const Edge<ET>* edges , const Node<NT>* nodes)
            : node_ (node) ,
            names_ (names) ,
            edges_ (edges) ,
            nodes_ (nodes)
        {}
        const_iterator () = default;
        const_iterator (const const_iterator& that) = default;
        const_iterator& operator=(const const_iterator& that) = default;

        int operator* () { return names_->GetInfo_NTO (node_); }
        void operator++() { ++node_; }
        const_iterator& operator++(int) { ++node_; return *this; }
        void operator--() { --node_; }
        const_iterator& operator--(int) { --node_; return *this; }
        explicit operator bool () const { return node_ == NOT_FILLED; }
        bool operator == (const const_iterator& that) const { return node_ == that.node_; }
        bool operator != (const const_iterator& that) const { return node_ != that.node_; }

        const_iterator_edge<ET> cbegin () const { return const_iterator_edge<ET> (nodes_[node_].next_ , edges_); }
        const_iterator_edge<ET> cend () const { return const_iterator_edge<ET> (NOT_FILLED , nullptr); }
    };
}