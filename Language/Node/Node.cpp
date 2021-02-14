#include "Node.hpp"
#include "Symtab.hpp"
#include "../Common_libs/errors.hpp"

namespace INode
{
    Symtab_t global_names;

    IScope_t *CreateScope() { return new Scope_t{nullptr}; }

    INode_t *CreateDouble(double val) { return new NodeDouble_t{val}; }
    INode_t *CreateName() { return new NodeName_t; }

    INode_t *CreateMathOp(
        INode_t *lhs, MathOp op, INode_t *rhs) { return new NodeMathOp_t{lhs, op, rhs}; }
    INode_t *CreateConditionOp(
        INode_t *lhs, ConditionOp op, INode_t *rhs /* = nullptr*/) { return new NodeConditionOp_t{lhs, op, rhs}; }
    INode_t *CreateCompOp(
        INode_t *lhs, ComparingOp op, INode_t *rhs) { return new NodeComp_t{lhs, op, rhs}; }
    INode_t *CreateCommonOp(
        INode_t *lhs, CommonOp op, INode_t *rhs /*= nullptr*/) { return new NodeCommonOp_t{lhs, op, rhs}; }

    INode_t *CreateIf(
        INode_t *condition, INode_t *scope) { return new NodeIf_t{condition, scope}; }
    INode_t *CreateWhile(
        INode_t *condition, INode_t *scope) { return new NodeWhile_t{condition, scope}; }

    INode_t::return_type_t Scope_t::calc()
    {
        for (const auto &branch : branches_)
            branch->calc();

        return 0;
    }

    INode_t *Scope_t::AddName(const std::string &name)
    {
        INode_t *name_p = Scope_t::GetName(name);
        if (name_p != nullptr)
            return name_p;

        INode_t *new_name = new NodeName_t;
        global_names.AddName(this, name, new_name);
        return new_name;
    }

    INode_t *Scope_t::GetName(const std::string &name)
    {
        INode_t *out = global_names.FindName(this, name);
        if (out != nullptr)
            return out;

        if (prev_scope_)
            out = prev_scope_->GetName(name);

        return out;
    }

    Scope_t::~Scope_t()
    {
        for (const auto &x : branches_)
            delete x;
    }

    INode_t::return_type_t NodeMathOp_t::calc()
    {
        switch (op_)
        {
        case MathOp::Plus:
            return lhs_->calc() + rhs_->calc();

        case MathOp::Minus:
            return lhs_->calc() - rhs_->calc();

        case MathOp::Div:
            return lhs_->calc() / rhs_->calc();
            break;

        case MathOp::Mult:
            return lhs_->calc() * rhs_->calc();
            break;
        default:
            break;
        }

        WARNING("Found not done operation!");
        return -1;
    }

    INode_t::return_type_t NodeConditionOp_t::calc()
    {
        switch (op_)
        {
        case ConditionOp::And:
            return lhs_->calc() && rhs_->calc();

        case ConditionOp::Or:
            return lhs_->calc() || rhs_->calc();

        case ConditionOp::Not:
            return !static_cast<bool>(lhs_->calc());
        default:
            break;
        }

        WARNING("Found not done operation!");
        return -1;
    }

    INode_t::return_type_t NodeComp_t::calc()
    {
        switch (op_)
        {
        case ComparingOp::Bigger:
            return lhs_->calc() > rhs_->calc();

        case ComparingOp::Less:
            return lhs_->calc() < rhs_->calc();

        case ComparingOp::BiggerEqual:
            return rhs_->calc() >= rhs_->calc();

        case ComparingOp::LessEqual:
            return lhs_->calc() <= rhs_->calc();

        case ComparingOp::Equal:
            return lhs_->calc() == rhs_->calc();

        case ComparingOp::NotEqual:
            return lhs_->calc() != rhs_->calc();
        default:
            break;
        }

        WARNING("Found not done operation!");
        return -1;
    }

    INode_t::return_type_t NodeCommonOp_t::calc()
    {
        double read_double = 0;

        switch (op_)
        {
        case CommonOp::Output:
            std::cout << lhs_->calc() << std::endl;
            if (!std::cout.good())
            {
                WARNING("Writing was unseccessful");
                return -1;
            }

            return 0;

        case CommonOp::Input:
            std::cin >> read_double;

            if (!std::cin.good())
            {
                WARNING("Reading was unseccessful");
                return -1;
            }

            if (lhs_ != nullptr)
            {
                NodeName_t *name = static_cast<NodeName_t *>(lhs_);
                name->set(read_double);
            }

            return read_double;

        case CommonOp::Assign:
            read_double = rhs_->calc();
            static_cast<NodeName_t *>(lhs_)->set(read_double);
            return read_double;
        default:
            break;
        }

        ERROR("Found not done operation!");
        return -1;
    }

    INode_t::return_type_t NodeIf_t::calc()
    {
        if (condition_->calc())
            scope_->calc();

        return 0;
    }

    INode_t::return_type_t NodeWhile_t::calc()
    {
        while (condition_->calc())
            scope_->calc();

        return 0;
    }

    ComparingOp DefineCompType(const std::string &token)
    {
        if (token.empty())
        {
            WARNING("Empty token!");
            return ComparingOp::Nothing;
        }

        switch (token[0])
        {

        case '>':
            if (token.size() == 1)
                return ComparingOp::Bigger;
            if (token.size() == 2 && token[1] != '=')
            {
                WARNING("Not a token!");
                return ComparingOp::Nothing;
            }
            return ComparingOp::BiggerEqual;
            break;

        case '<':
            if (token.size() == 1)
                return ComparingOp::Less;
            if (token.size() == 2 && token[1] != '=')
            {
                WARNING("Not a token!");
                return ComparingOp::Nothing;
            }
            return ComparingOp::LessEqual;
            break;

        default:
            if (token == "==")
                return ComparingOp::Equal;

            if (token == "!=")
                return ComparingOp::NotEqual;

            break;
        }

        WARNING("Not a token!");
        return ComparingOp::Nothing;
    }
} // namespace INode