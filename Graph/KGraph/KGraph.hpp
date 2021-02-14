#pragma once

#include <iostream>
#include <vector>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <fstream>
#include "../Common_libs/Errors/Errors.hpp"
#include "../Common_libs/Color.hpp"

namespace MCGraph
{
    class NamesEq_t final
    {
    public:
        NamesEq_t() = default;
        //  returns true if element was inserted
        bool AddInfo_NTO(int new_node, int old_node);
        int GetInfo_NTO(int node) const;
        int GetInfo_NTO(size_t node) const;

        bool AddInfo_OTN(int old_node, int new_node);
        int GetInfo_OTN(int node) const;
        int GetInfo_OTN(size_t node) const;

    private:
        std::unordered_map<int, int> new_TO_old_;
        std::unordered_map<int, int> old_TO_new_;
    };
} // namespace MCGraph

//Mark Class
namespace MC
{
    //NT - Node type, ET - Edge type
    template <class ET = int, class NT = int>
    class KGraph
    {
    public:
        KGraph() = default;
        KGraph(const KGraph &) = delete;
        KGraph(KGraph &&) = delete;

        KGraph(std::initializer_list<std::initializer_list<int>> data);

        void AddEdge(int left, int right, ET data = ET());

        //!Debug
        /////////////////////////////////////////////////////////////////
        explicit operator bool() const noexcept { return !nodes_.empty(); }
        bool empty() const noexcept { return nodes_.empty(); }

        void dump(std::ostream &out) const;
        /////////////////////////////////////////////////////////////////

        //!WFS + DFS
        /////////////////////////////////////////////////////////////////
        std::vector<std::pair<int, int>> dumpWFS(int start_node = 0) const;

        enum class Color
        {
            Blue = 0,
            Red
        };
        std::vector<std::pair<int, Color>> dumpDFS(bool *is_2 = nullptr) const;
        /////////////////////////////////////////////////////////////////

    private:
        MCGraph::NamesEq_t names_nodes_;

        struct Node
        {
            int next_;
            int prev_;
        };
        std::vector<Node> nodes_;
        struct Edge
        {
            int node_;
            int next_;
            int prev_;
            ET data_;
        };
        std::vector<Edge> edges_;

        void dumpWFS_Node(int node, int width, std::unordered_map<int, int> &marked_nodes) const;
        bool dumpDFS_Node(int node, std::unordered_map<int, Color> &marked_nodes, std::vector<std::pair<int, Color>> &queue, Color color) const;

        int PairNode(int node) const;
        Color OtherColor(Color color) const noexcept;

        static constexpr int END_FIND = -1;
    };

} // namespace MC
// operator << is below!

template <class ET, class NT>
MC::KGraph<ET, NT>::KGraph(std::initializer_list<std::initializer_list<int>> data)
{
    //!Rename nodes
    ////////////////////////////////////////////////
    int cur_sz = 0; //count of Nodes
    for (auto pair : data)
    {
        if (pair.size() != 2)
            ERROR("Found not a pair - can't be initialized!\n");

        for (auto elem : pair) //now pair.size() == 2
        {
            if (names_nodes_.AddInfo_OTN(elem, cur_sz))
            {
                names_nodes_.AddInfo_NTO(cur_sz, elem);
                cur_sz++; //insert was succesful
            }
        }
    }

    nodes_.resize(cur_sz);
    edges_.reserve(2 * data.size());
    int edge_size = 2 * data.size();
    ////////////////////////////////////////////////

    //!Preparing Edges and Edge_Info
    ////////////////////////////////////////////////
    for (auto pair : data)
    {
        auto iter = pair.begin();
        int node_1 = names_nodes_.GetInfo_OTN(*iter);
        ++iter;
        int node_2 = names_nodes_.GetInfo_OTN(*iter);

        edges_.push_back({node_1, 0, 0, ET()});
        edges_.push_back({node_2, 0, 0, ET()});
    }
    ////////////////////////////////////////////////

    //!Preparing Next
    ////////////////////////////////////////////////
    for (int i = 0; i < cur_sz; ++i)
    {
        int last_found = END_FIND;

        for (int j = 0; j < edge_size; ++j)
        {
            if (edges_[j].node_ == i)
            {
                if (last_found == END_FIND)
                    nodes_[i].next_ = j;
                else
                    edges_[last_found].next_ = j;

                edges_[j].next_ = END_FIND;
                last_found = j;
            }
        }
    }
    ////////////////////////////////////////////////

    //!Preparing Prev
    ////////////////////////////////////////////////
    for (int i = 0; i < cur_sz; ++i)
    {
        int last_found = END_FIND;

        for (int j = edge_size - 1; j >= 0; --j)
        {
            if (edges_[j].node_ == i)
            {
                if (last_found == END_FIND)
                    nodes_[i].prev_ = j;
                else
                    edges_[last_found].prev_ = j;

                edges_[j].prev_ = END_FIND;
                last_found = j;
            }
        }
    }
    ////////////////////////////////////////////////
}

template <class T1, class T2>
void MC::KGraph<T1, T2>::AddEdge(int left, int right, T1 data/* = T1()*/)
{
    int new_left = -1, new_right = -1;
    int edge_size = static_cast<int>(edges_.size());
    int node_size = static_cast<int>(nodes_.size());

    if (names_nodes_.AddInfo_OTN(left, node_size))
    {
        names_nodes_.AddInfo_NTO(node_size, left);
        new_left = node_size;
        nodes_.push_back({edge_size, edge_size});
        node_size++;
    }
    else
        new_left = names_nodes_.GetInfo_OTN(left);

    if (names_nodes_.AddInfo_OTN(right, node_size))
    {
        names_nodes_.AddInfo_NTO(node_size, right);
        new_right = node_size;
        nodes_.push_back({edge_size + 1, edge_size + 1});
        node_size++;
    }
    else
        new_right = names_nodes_.GetInfo_OTN(right);

    //Common prepa
    edges_.push_back({new_left, END_FIND, 0, data});
    edges_.push_back({new_right, END_FIND, 0, data});
    edge_size += 2;

    nodes_[new_left].prev_ = edge_size - 2;
    nodes_[new_right].prev_ = edge_size - 1;

    if (nodes_[new_left].next_ == edge_size - 2)
        edges_[edge_size - 2].prev_ = END_FIND;
    else
    {
        int cur_node = nodes_[new_left].next_;
        while (edges_[cur_node].next_ != END_FIND)
            cur_node = edges_[cur_node].next_;

        edges_[cur_node].next_ = edge_size - 2;
        edges_[edge_size - 2].prev_ = cur_node;
    }

    if (nodes_[new_right].next_ == edge_size - 1)
        edges_[edge_size -1].prev_ = END_FIND;
    else
    {
        int cur_node = nodes_[new_right].next_;
        while (edges_[cur_node].next_ != END_FIND)
            cur_node = edges_[cur_node].next_;

        edges_[cur_node].next_ = edge_size - 1;
        edges_[edge_size - 1].prev_ = cur_node;
    }
}

template <class T1, class T2>
void MC::KGraph<T1, T2>::dump(std::ostream &out) const
{
    out << MLib::Color::Under_Purple << "\nDump is starting!\n";
    out << "Nodes:\n"
        << MLib::Color::Green;
    out << "|Node\t|Next\t|Prev\t|\n";
    out << "-------------------------\n";

    for (size_t i = 0; i < nodes_.size(); ++i)
        out << "|" << names_nodes_.GetInfo_NTO(i) << "\t| " << nodes_[i].next_ << "\t| " << nodes_[i].prev_ << "\t|\n";

    out << MLib::Color::Under_Purple << "Edges:\n"
        << MLib::Color::Green;
    out << "|Node\t|Next\t|Prev\n";
    out << "-------------------------\n";

    for (size_t i = 0; i < edges_.size(); ++i)
        out << "|" << edges_[i].node_ << "\t| " << edges_[i].next_ << "\t| " << edges_[i].prev_ << "\t|\n";

    out << MLib::Color::Under_Purple << "Dump is ended!\n\n"
        << MLib::Color::Reset;
}

// //!WFS
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template <class T1, class T2>
std::vector<std::pair<int, int>> MC::KGraph<T1, T2>::dumpWFS(int start_node /* = 0*/) const
{
    if (empty())
    {
        WARNING("Tried to get dumpWFS from empty graph!\n");
        return std::vector<std::pair<int, int>>{};
    }

    std::unordered_map<int, int> marked_nodes; //this not empty
    marked_nodes.insert({start_node, 0});

    dumpWFS_Node(start_node, 0, marked_nodes);

    std::vector<std::pair<int, int>> output;
    output.reserve(nodes_.size());
    for (size_t i = 0; i < nodes_.size(); ++i)
        output.push_back({names_nodes_.GetInfo_NTO(i), marked_nodes[i]});

    return output;
}

template <class T1, class T2>
void MC::KGraph<T1, T2>::dumpWFS_Node(int node, int width, std::unordered_map<int, int> &marked_nodes) const
{
    std::vector<int> next_nodes;
    int cur_pos = nodes_[node].next_;

    while (cur_pos != END_FIND)
    {
        int pair = PairNode(cur_pos);

        auto finding = marked_nodes.insert({pair, width + 1});
        if (finding.second)
            next_nodes.push_back(pair);
        else if (finding.first->second > width + 1) //element already was added
            marked_nodes[pair] = width + 1;         //existed smaller way to this Node

        cur_pos = edges_[cur_pos].next_;
    }

    for (auto nd : next_nodes)
        dumpWFS_Node(nd, width + 1, marked_nodes);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// //?WFS

//!DFS
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template <class T1, class T2>
std::vector<std::pair<int, typename MC::KGraph<T1, T2>::Color>>
MC::KGraph<T1, T2>::dumpDFS(bool *is_2 /*= nullptr*/) const
{
    if (empty())
    {
        WARNING("Tried to get dumpDFS from empty graph!\n");
        return std::vector<std::pair<int, Color>>{};
    }

    //Preparing data
    std::unordered_map<int, Color> marked_nodes;
    marked_nodes.insert({0, Color::Blue});
    std::vector<std::pair<int, Color>> queue;
    queue.reserve(nodes_.size());

    bool is_2_out = dumpDFS_Node(0, marked_nodes, queue, Color::Red);
    for (size_t i = 1; i < nodes_.size(); ++i)
        if (marked_nodes.find(i) == marked_nodes.end())
            if (!dumpDFS_Node(i, marked_nodes, queue, Color::Red))
                is_2_out = false;
    
    if (is_2 != nullptr)
        *is_2 = is_2_out;

    for (auto &pair : queue)
        pair.first = names_nodes_.GetInfo_NTO(pair.first);

    return queue;
}

template <class T1, class T2>
bool MC::KGraph<T1, T2>::dumpDFS_Node(int node, std::unordered_map<int, Color> &marked_nodes, std::vector<std::pair<int, Color>> &queue, Color color) const
{
    queue.push_back({node, OtherColor(color)});
    int cur_pos = nodes_[node].next_;
    bool is_2 = true;

    while (cur_pos != END_FIND)
    {
        int pair = PairNode(cur_pos);

        auto check = marked_nodes.insert({pair, color});
        if (check.second)
        {
            if (!dumpDFS_Node(pair, marked_nodes, queue, OtherColor(color))) //element was pushed
                is_2 = false;
        }
        else if (check.first->second != color) //element was already inserted
        {
            //std::cout << "Color: " << names_nodes_.GetInfo_NTO(check.first->first) << std::endl;
            is_2 = false;
        }

        cur_pos = edges_[cur_pos].next_;
    }

    return is_2;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//?DFS

template <class T1, class T2>
int MC::KGraph<T1, T2>::PairNode(int node) const
{
    return (node % 2) ? edges_[node - 1].node_ : edges_[node + 1].node_;
}

template <class T1, class T2>
std::ostream &operator<<(std::ostream &out, const MC::KGraph<T1, T2> &graph)
{
    graph.dump(out);
    return out;
}

template <class T1, class T2>
typename MC::KGraph<T1, T2>::Color
MC::KGraph<T1, T2>::OtherColor(Color color) const noexcept
{
    return static_cast<Color>(1 - static_cast<int>(color));
}