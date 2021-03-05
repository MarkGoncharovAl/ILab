#pragma once
#include "KGraph.dec"


//Этот метод скоро удалю, но хочу сделать удобный конструктор графа
//главное, что хочется - доавляю парами для рёбер, плюс какая то информация
//то есть неопределённое кол-во ребёр с неопределённым кол-вом данных в них...
//Звучит страшно, а реализовывать вообще хз как)
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

template <class T1 , class T2>
void MC::KGraph<T1 , T2>::AddEdge (int left , int right , T1 data/* = T1()*/)
{
    int new_left = -1 , new_right = -1;
    int edge_size = static_cast<int>(edges_.size ());
    int node_size = static_cast<int>(nodes_.size ());

    if (names_nodes_.AddInfo_OTN (left , node_size))
    {
        names_nodes_.AddInfo_NTO (node_size , left);
        new_left = node_size;
        nodes_.push_back ({ edge_size, edge_size, MCGraph::Color{} });
        node_size++;
    }
    else
        new_left = names_nodes_.GetInfo_OTN (left);

    if (names_nodes_.AddInfo_OTN (right , node_size))
    {
        names_nodes_.AddInfo_NTO (node_size , right);
        new_right = node_size;
        nodes_.push_back ({ edge_size + 1, edge_size + 1, MCGraph::Color{} });
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
    //Оставлю раскраску, это ж только для дебага... В main конечно её нет
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

// //!WFS
//!Лучше не смотреть. Хоть и работает, но старая версия, бесполезная
//В основном использовалась для понимания графа, сначала хотел писать 
//с её помощью
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template <class T1 , class T2>
std::vector<std::pair<int , int>> MC::KGraph<T1 , T2>::dumpWFS (int start_node /* = 0*/) const
{
    if (empty ())
    {
        WARNING ("Tried to get dumpWFS from empty graph!\n");
        return std::vector<std::pair<int , int>>{};
    }

    std::unordered_map<int , int> marked_nodes; //this not empty
    marked_nodes.insert ({ start_node, 0 });

    dumpWFS_Node (start_node , 0 , marked_nodes);

    std::vector<std::pair<int , int>> output;
    output.reserve (nodes_.size ());
    for (size_t i = 0; i < nodes_.size (); ++i)
        output.push_back ({ names_nodes_.GetInfo_NTO (i), marked_nodes[i] });

    return output;
}

template <class T1 , class T2>
void MC::KGraph<T1 , T2>::dumpWFS_Node (int node , int width , std::unordered_map<int , int>& marked_nodes) const
{
    std::vector<int> next_nodes;
    int cur_pos = nodes_[node].next_;

    while (cur_pos != END_FIND)
    {
        int pair = PairNode (cur_pos);

        auto finding = marked_nodes.insert ({ pair, width + 1 });
        if (finding.second)
            next_nodes.push_back (pair);
        else if (finding.first->second > width + 1) //element already was added
            marked_nodes[pair] = width + 1;         //existed smaller way to this Node

        cur_pos = edges_[cur_pos].next_;
    }

    for (auto nd : next_nodes)
        dumpWFS_Node (nd , width + 1 , marked_nodes);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
// //?WFS

//!DFS
//ГЛАВНОЕ
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
template <class T1 , class T2>
MCGraph::DFS_Bip_Out MC::KGraph<T1 , T2>::DFS_Bip () 
{
    if (empty ())
    {
        WARNING ("Tried to get dumpDFS from empty graph!\n");
        return MCGraph::DFS_Bip_Out{true, {}};
    }

    // Проверка на то, что хранится в вершине именно цвет.
    // Для удобства вывода, хотя, не спорю, нелогично
    // Потому что пользователь захочет, возможно, хранить
    // ещё данные. Думаю, чуть переделаю.
    if (typeid(T2) != typeid(MCGraph::Color))
    {
        WARNING ("Type of node isn't color!");
        return MCGraph::DFS_Bip_Out{false, {}};
    }

    for (Node& node : nodes_)
        node.data_.data_ = MCGraph::Color::START_VALUE;

    //Проверка на несолько компонент связности
    MCGraph::DFS_Bip_Out output = {true, {}};
    for (size_t i = 0; i < nodes_.size (); ++i)
        if (nodes_[i].data_.empty())
            if ((output = DFS_Bip_Node (i)).is_biparatite_ == false)
                break;

    return output;
}

template <class T1 , class T2>
MCGraph::DFS_Bip_Out MC::KGraph<T1 , T2>::DFS_Bip_Node (int node) 
{
    //used for traсe of unbipartite - 
    //как в упражнении Кнута
    std::vector<int> nodes_parents(nodes_.size(), -1);
    ////////

    MCGraph::DFS_Bip_Out output = {true, {}};

    //Нерассмотренные вершины
    std::stack<int> considered_nodes;
    considered_nodes.push(node);
    nodes_[node].data_.data_ = MCGraph::Color::BLUE;

    while (!considered_nodes.empty())
    {
        //начинаем раскручивать стек
        int cur_node = considered_nodes.top();
        considered_nodes.pop();

        //возвращает противоположный цвет
        MCGraph::Color::type cur_color = nodes_[cur_node].data_.get_another();

        //копаем все соседние рёбра
        int cur_pos = nodes_[cur_node].next_; 

        while (cur_pos != END_FIND)
        {
            //возвращает номер вершины, которая соединена с помощью
            //рассматриваемого ребра
            int pair = PairNode (cur_pos);
            MCGraph::Color& checking = nodes_[pair].data_;

            if (checking.empty())
            {
                //ещё не помечали вершину -> заносим в стэк, красим, пишем родителя
                checking.data_ = cur_color;
                considered_nodes.push(pair);
                nodes_parents[pair] = cur_node;
            }

            else if (checking.data_ != cur_color) //element was already inserted
            {
                //getting back using parent's nodes
                output.is_biparatite_ = false;

                //preparing nodes in cycle
                //Это три вершины, точно входящие в цикл нечётной длины
                int parent_of_pair = nodes_parents[pair];
                output.prove_.push_back(names_nodes_.GetInfo_NTO(parent_of_pair));
                output.prove_.push_back(names_nodes_.GetInfo_NTO(pair));
                output.prove_.push_back(names_nodes_.GetInfo_NTO(cur_node));
                ////////////////////////////////////////////

                //debug
                // for (const int elem : output.prove_)
                //     std::cout << elem << " ";
                // std::cout << "\n";
                ////////////////////////////////////////////

                //проверяем на больший цикл
                while ((cur_node = nodes_parents[cur_node]) != parent_of_pair)
                {
                    output.prove_.push_back(names_nodes_.GetInfo_NTO(cur_node));

                    //debug
                    ////////////////////////////////////////////
                    // for (const int elem : output.prove_)
                    //     std::cout << elem << " ";
                    // std::cout << "\n";
                    ////////////////////////////////////////////
                }

                return output;
            }

            cur_pos = edges_[cur_pos].next_;
        }
    }

    return output;
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
//?DFS

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

template <class T1 , class T2>
std::vector<std::pair<int , MCGraph::Color>> 
MC::KGraph<T1 , T2>::GetColors() const
{
    std::vector<std::pair<int , MCGraph::Color>> out;
    out.reserve(nodes_.size ());

    for (size_t i = 0; i < nodes_.size (); ++i)
        out.emplace_back(names_nodes_.GetInfo_NTO(i), nodes_[i].data_);
    
    return out;
}