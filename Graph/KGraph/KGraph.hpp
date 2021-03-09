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

//!DECALARATIONS
namespace MCGraph
{
    //I use it, because it's so convenient.
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

    //to colorize nodes
    struct Color
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
        bool change_color() noexcept;

        //out char of data_ to print in std::cout ('b' or 'r')
        char print () const noexcept;

        //returns differ color 
        char get_another() const noexcept;

        bool operator == (const Color &that) const noexcept {return data_ == that.data_;}
    
        type data_ = START_VALUE;
    };
} // namespace MCGraph

//Mark Class -> MC
namespace MC
{
    //NT - Node type, ET - Edge type
    template <class ET = int , class NT = MCGraph::Color>
    class KGraph
    {
    public:
        //without it doesn't work:
        //MC::KGraph<> graph;
        KGraph() = default;

        KGraph (std::initializer_list<std::initializer_list<int>> data);

        void AddEdge (int left , int right , ET data = ET ());

        //!Debug
        /////////////////////////////////////////////////////////////////
        explicit operator bool () const noexcept { return !nodes_.empty (); }
        bool empty () const noexcept { return nodes_.empty (); }

        void dump (std::ostream& out) const;
        /////////////////////////////////////////////////////////////////

        //!DFS
        /////////////////////////////////////////////////////////////////
        //returns Nodes' colors in Graph
        std::vector<std::pair<int , MCGraph::Color>> GetColors();
        bool IsBipartite();
        std::vector<int> ProveNotBipartite();
        /////////////////////////////////////////////////////////////////

    private:
        MCGraph::NamesEq_t names_nodes_;

        struct Node
        {
            int next_ = -1;
            int prev_ = -1;
            NT data_;
            MCGraph::Color color_;
        };
        std::vector<Node> nodes_;
        struct Edge
        {
            int node_ = -1;
            int next_ = -1;
            int prev_ = -1;
            ET data_;
        };
        std::vector<Edge> edges_;

        // to control functions <is_bipartite, GetColors>
        ////////////////
        enum class bipar {NOT_CORRECT, TRUE, FALSE};
        bipar is_colorized_ = bipar::NOT_CORRECT;
        ////////////////

        // DFS functions of coloring nodes
        ////////////////////////////////////
        std::vector<int> ColorizeNodes ();
        std::vector<int> ColorizeNodes_Component (int node);
        ////////////////////////////////////

        int PairNode (int node) const;

        static constexpr int END_FIND = -1;
    };

} // namespace MC
// operator << is below!


///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////


template <class ET , class NT>
MC::KGraph<ET , NT>::KGraph (std::initializer_list<std::initializer_list<int>> data)
{
    //!Rename nodes
    ////////////////////////////////////////////////
    int cur_sz = 0; //count of Nodes
    for (auto pair : data)
    {
        if (pair.size () != 2)
            ERROR ("Found not a pair - can't be initialized!\n");

        for (auto elem : pair) //now pair.size() == 2
        {
            if (names_nodes_.AddInfo_OTN (elem , cur_sz))
            {
                names_nodes_.AddInfo_NTO (cur_sz , elem);
                cur_sz++; //insert was succesful
            }
        }
    }

    nodes_.resize (cur_sz);
    edges_.reserve (2 * data.size ());
    int edge_size = 2 * data.size ();
    ////////////////////////////////////////////////

    //!Preparing Edges and Edge_Info
    ////////////////////////////////////////////////
    for (auto pair : data)
    {
        auto iter = pair.begin ();
        int node_1 = names_nodes_.GetInfo_OTN (*iter);
        ++iter;
        int node_2 = names_nodes_.GetInfo_OTN (*iter);

        edges_.push_back ({ node_1, 0, 0, ET () });
        edges_.push_back ({ node_2, 0, 0, ET () });
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



///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

template <class T1 , class T2>
void MC::KGraph<T1 , T2>::AddEdge (int left , int right , T1 data/* = T1()*/)
{
    //set flag that graph should be recolored again!
    is_colorized_ = bipar::NOT_CORRECT;

    int new_left = -1 , new_right = -1;
    int edge_size = edges_.size ();
    int node_size = nodes_.size ();

    if (names_nodes_.AddInfo_OTN (left , node_size))
    {
        names_nodes_.AddInfo_NTO (node_size , left);
        new_left = node_size;
        nodes_.push_back ({ edge_size, edge_size, T2{}, MCGraph::Color{} });
        node_size++;
    }
    else
        new_left = names_nodes_.GetInfo_OTN (left);

    if (names_nodes_.AddInfo_OTN (right , node_size))
    {
        names_nodes_.AddInfo_NTO (node_size , right);
        new_right = node_size;
        nodes_.push_back ({ edge_size + 1, edge_size + 1, T2{}, MCGraph::Color{} });
        node_size++;
    }
    else
        new_right = names_nodes_.GetInfo_OTN (right);

    //Common prepa
    edges_.push_back ({ new_left, END_FIND, 0, data });
    edges_.push_back ({ new_right, END_FIND, 0, data });
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
        edges_[edge_size - 1].prev_ = END_FIND;
    else
    {
        int cur_node = nodes_[new_right].next_;
        while (edges_[cur_node].next_ != END_FIND)
            cur_node = edges_[cur_node].next_;

        edges_[cur_node].next_ = edge_size - 1;
        edges_[edge_size - 1].prev_ = cur_node;
    }
}

template <class T1 , class T2>
void MC::KGraph<T1 , T2>::dump (std::ostream& out) const
{
    out << MLib::Color::Under_Purple << "\nDump is starting!\n";
    out << "Nodes:\n"
        << MLib::Color::Green;
    out << "|Node\t|Next\t|Prev\t|\n";
    out << "-------------------------\n";

    for (size_t i = 0; i < nodes_.size (); ++i)
        out << "|" << names_nodes_.GetInfo_NTO (i) << "\t| " << nodes_[i].next_ << "\t| " << nodes_[i].prev_ << "\t|\n";

    out << MLib::Color::Under_Purple << "Edges:\n"
        << MLib::Color::Green;
    out << "|Node\t|Next\t|Prev\n";
    out << "-------------------------\n";

    for (size_t i = 0; i < edges_.size (); ++i)
        out << "|" << edges_[i].node_ << "\t| " << edges_[i].next_ << "\t| " << edges_[i].prev_ << "\t|\n";

    out << MLib::Color::Under_Purple << "Dump is ended!\n\n"
        << MLib::Color::Reset;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//!DFS
template <class T1 , class T2>
std::vector<int> MC::KGraph<T1 , T2>::ColorizeNodes () 
{
    if (empty ())
    {
        WARNING ("Tried to get dumpDFS from empty graph!\n");
        return std::vector<int>{};
    }

    for (Node& node : nodes_)
        node.color_.data_ = MCGraph::Color::START_VALUE;

    is_colorized_ = bipar::TRUE;
    std::vector<int> output;

    //checking for several components
    for (size_t i = 0; i < nodes_.size (); ++i)
        if (nodes_[i].color_.empty() &&
           (output = ColorizeNodes_Component (i)).empty() == false)
                break;

    return output;
}

template <class T1 , class T2>
std::vector<int> MC::KGraph<T1 , T2>::ColorizeNodes_Component (int node) 
{
    //output - cycle of odd length
    std::vector<int> proving;

    //used for traсe of unbipartite - 
    //like Knut
    std::vector<int> nodes_parents(nodes_.size(), -1);

    //Not colorized nodes
    std::stack<int> considered_nodes;
    considered_nodes.push(node);
    nodes_[node].color_.data_ = MCGraph::Color::BLUE;

    while (!considered_nodes.empty())
    {
        //beginning to release stack
        int cur_node = considered_nodes.top();
        considered_nodes.pop();

        MCGraph::Color::type cur_color = nodes_[cur_node].color_.get_another();

        //looking to every neighboor
        int cur_pos = nodes_[cur_node].next_; 
        while (cur_pos != END_FIND)
        {
            int pair = PairNode (cur_pos);
            MCGraph::Color& checking = nodes_[pair].color_;

            if (checking.empty())
            {
                //haven't colored -> colorize and write a parent
                checking.data_ = cur_color;
                considered_nodes.push(pair);
                nodes_parents[pair] = cur_node;
            }

            else if (checking.data_ != cur_color) //element was already inserted
            {
                //getting back using parent's nodes
                is_colorized_ = bipar::FALSE;

                //preparing nodes in cycle
                int parent_of_pair = nodes_parents[pair];

                if (parent_of_pair == -1)
                {
                    proving.push_back(names_nodes_.GetInfo_NTO(pair));
                    proving.push_back(names_nodes_.GetInfo_NTO(cur_node));
                }
                else // parent_of_pair != -1
                {
                    //This points are obviously in the cycle
                    proving.push_back(names_nodes_.GetInfo_NTO(parent_of_pair));
                    proving.push_back(names_nodes_.GetInfo_NTO(pair));
                    proving.push_back(names_nodes_.GetInfo_NTO(cur_node));
                    ////////////////////////////////////////////

                    //cheking for big cycle
                    while ((cur_node = nodes_parents[cur_node]) != parent_of_pair)
                        proving.push_back(names_nodes_.GetInfo_NTO(cur_node));
                }
                return proving;
            }

            cur_pos = edges_[cur_pos].next_;
        }
    }

    return proving;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

template <class T1 , class T2>
int MC::KGraph<T1 , T2>::PairNode (int node) const
{
    return (node % 2) ? edges_[node - 1].node_ : edges_[node + 1].node_;
}

template <class T1 , class T2>
std::ostream& operator<<(std::ostream& out , const MC::KGraph<T1 , T2>& graph)
{
    graph.dump (out);
    return out;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////
//!Interface for ColorizeNodes

template <class T1 , class T2>
std::vector<std::pair<int , MCGraph::Color>> 
MC::KGraph<T1 , T2>::GetColors()
{
    if (is_colorized_ == bipar::NOT_CORRECT)
        ColorizeNodes();

    std::vector<std::pair<int , MCGraph::Color>> out;
    out.reserve(nodes_.size ());

    for (size_t i = 0; i < nodes_.size (); ++i)
        out.emplace_back(names_nodes_.GetInfo_NTO(i), nodes_[i].color_);
    
    return out;
}

template <class T1 , class T2>
bool MC::KGraph<T1, T2>::IsBipartite() 
{
    if (is_colorized_ == bipar::NOT_CORRECT)
        ColorizeNodes();

    switch(is_colorized_)
    {
    case bipar::FALSE:
        return false;
        break;
    case bipar::TRUE:
        return true;
        break;
    case bipar::NOT_CORRECT:
    default:
        WARNING ("Not appropriate case of bipartite!");
    }
    return false;
}

template <class T1 , class T2>
std::vector<int> MC::KGraph<T1, T2>::ProveNotBipartite()
{
    return ColorizeNodes();
}