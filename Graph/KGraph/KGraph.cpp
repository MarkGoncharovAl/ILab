#include "KGraph.hpp"

bool MCGraph::NamesEq_t::AddInfo_NTO(int new_node, int old_node)
{
    return new_TO_old_.insert({new_node, old_node}).second;
}
int MCGraph::NamesEq_t::GetInfo_NTO(int node) const
{
    auto iter = new_TO_old_.find(node);
    if (iter != new_TO_old_.end())
        return iter->second;
    else
        WARNING("Can't GetInfo NTO from the node " + std::to_string(node));

    return 0;
}
int MCGraph::NamesEq_t::GetInfo_NTO(size_t node) const
{
    return GetInfo_NTO(static_cast<int>(node));
}

bool MCGraph::NamesEq_t::AddInfo_OTN(int new_node, int old_node)
{
    return old_TO_new_.insert({new_node, old_node}).second;
}
int MCGraph::NamesEq_t::GetInfo_OTN(int node) const
{
    auto iter = old_TO_new_.find(node);
    if (iter != old_TO_new_.end())
        return iter->second;
    else
        WARNING("Can't GetInfo OTN from the node " + std::to_string(node));

    return 0;
}
int MCGraph::NamesEq_t::GetInfo_OTN(size_t node) const
{
    return GetInfo_OTN(static_cast<int>(node));
}