#include "Symtab.hpp"

namespace INode
{
    bool SymtabLoc_t::AddName(const std::string &name, INode_t *node, bool check /* = false*/)
    {
        if (!check &&
            names_.find(name) != names_.end())
            return false;

        names_.insert(std::make_pair(name, node));
        return true;
    }

    INode_t *SymtabLoc_t::FindName(const std::string &name)
    {
        auto check = names_.find(name);
        if (check == names_.end())
        {
            return nullptr;
        }

        return check->second;
    }

    SymtabLoc_t::~SymtabLoc_t()
    {
        for (auto &pair : names_)
        {
            delete pair.second;
        }
    }

    void Symtab_t::AddName(IScope_t *scope, const std::string &symbol, INode_t *n)
    {
        auto cur_sym = data_names_.find(scope);
        if (cur_sym == data_names_.end())
        {
            //hasn't been created cur_scope
            data_names_.insert(std::make_pair(scope, std::move(SymtabLoc_t{})));
            cur_sym = data_names_.find(scope);
        }
        (cur_sym->second).AddName(symbol, n);
    }

    INode_t *Symtab_t::FindName(IScope_t *const scope, std::string symbol)
    {
        auto cur_sym = data_names_.find(scope);
        if (cur_sym == data_names_.end())
        {
            //hasn't been created cur_scope
            data_names_.insert(std::make_pair(scope, std::move(SymtabLoc_t{})));
            return nullptr;
        }
        return cur_sym->second.FindName(symbol);
    }
} // namespace INode