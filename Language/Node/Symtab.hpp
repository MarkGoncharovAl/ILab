#pragma once

#include <string>
#include <unordered_map>
#include "INode.hpp"

namespace INode
{

    //////////
    //  !Local variables
    //////////
    class SymtabLoc_t final
    {
    public:
        ////////
        //  !returns true if operation was done
        //  !change check = true if you realise that name wasn't declared
        ////////
        bool AddName(const std::string &name, INode_t *node, bool check = false);

        //////////
        //  !Return nullptr if wasn't found!
        //////////
        INode_t *FindName(const std::string &name);

        ~SymtabLoc_t();

    private:
        std::unordered_map<std::string, INode_t *> names_;
    };

    class Symtab_t final
    {
    public:
        void AddName(IScope_t *scope, const std::string &symbol, INode_t *n);

        INode_t *FindName(IScope_t *const scope, std::string symbol);

    private:
        std::unordered_map<IScope_t *, SymtabLoc_t> data_names_;
    };

} // namespace INode