#include "KGraph.hpp"

bool MCGraph::NamesEq_t::AddInfo_NTO (int new_node , int old_node)
{
    return new_TO_old_.insert ({ new_node, old_node }).second;
}
int MCGraph::NamesEq_t::GetInfo_NTO (int node) const
{
    auto iter = new_TO_old_.find (node);
    if (iter != new_TO_old_.end ())
        return iter->second;
    else
        WARNING ("Can't GetInfo NTO from the node " + std::to_string (node));

    return 0;
}
int MCGraph::NamesEq_t::GetInfo_NTO (size_t node) const
{
    return GetInfo_NTO (static_cast<int>(node));
}

bool MCGraph::NamesEq_t::AddInfo_OTN (int new_node , int old_node)
{
    return old_TO_new_.insert ({ new_node, old_node }).second;
}
int MCGraph::NamesEq_t::GetInfo_OTN (int node) const
{
    auto iter = old_TO_new_.find (node);
    if (iter != old_TO_new_.end ())
        return iter->second;
    else
        WARNING ("Can't GetInfo OTN from the node " + std::to_string (node));

    return 0;
}
int MCGraph::NamesEq_t::GetInfo_OTN (size_t node) const
{
    return GetInfo_OTN (static_cast<int>(node));
}




bool MCGraph::Color::change_color () noexcept
{
    if (empty ())
        return false;
    data_ = 1 - data_;
    return true;
}

char MCGraph::Color::print () const noexcept
{
    switch (data_)
    {
    case BLUE:
        return 'b';
    case RED:
        return 'r';
    default:
        WARNING ("Can't print empty color!");
    }
    return 0;
}

char MCGraph::Color::get_another() const noexcept
{
    if (empty ())
    {
        WARNING ("Can't get another color!");
        return START_VALUE;
    }
    return (1 - data_);
}