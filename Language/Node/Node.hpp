#pragma once

#include <iostream>
#include <map>
#include <string>
#include <typeinfo>
#include <vector>

#include "INode.hpp"

namespace INode
{

    class Scope_t final : public IScope_t
    {
    private:
        std::vector<INode_t *> branches_;
        IScope_t *prev_scope_ = nullptr;

    public:
        return_type_t calc() override;

        IScope_t *Duplicate() override { return new Scope_t(this); }
        IScope_t *Reset() const override { return prev_scope_; }
        void AddBranch(INode_t *branch) override { branches_.push_back(branch); }
        INode_t *AddName(const std::string &name) override;
        INode_t *GetName(const std::string &name) override;

        Scope_t(Scope_t *prev_scope) : prev_scope_(prev_scope) {}
        Scope_t() = default;
        ~Scope_t() override;
    };

    class NodeDouble_t final : public INode_t
    {
    private:
        double val_;

    public:
        return_type_t calc() override { return val_; }
        NodeDouble_t(double val)
            : val_(val) {}
    };

    class NodeName_t final : public INode_t
    {
    private:
        double val_;

    public:
        return_type_t calc() override { return val_; };
        NodeName_t(double init_val)
            : val_(init_val) {}
        NodeName_t() = default;
        void set(double init_val) { val_ = init_val; }
    };

    class NodeMathOp_t final : public INode_t
    {
    private:
        INode_t *lhs_;
        INode_t *rhs_;
        MathOp op_;

    public:
        return_type_t calc() override;

        NodeMathOp_t(INode_t *lhs, MathOp op, INode_t *rhs)
            : lhs_(lhs),
              op_(op),
              rhs_(rhs) {}
        ~NodeMathOp_t() override
        {
            delete lhs_;
            delete rhs_;
        }
    };

    class NodeCommonOp_t final : public INode_t
    {
    private:
        INode_t *lhs_;
        INode_t *rhs_;
        CommonOp op_;

    public:
        return_type_t calc() override;

        NodeCommonOp_t(INode_t *lhs, CommonOp op, INode_t *rhs = nullptr)
            : lhs_(lhs),
              op_(op),
              rhs_(rhs) {}
        ~NodeCommonOp_t() override
        {
            delete lhs_;
            delete rhs_;
        }
    };

    class NodeConditionOp_t final : public INode_t
    {
    private:
        INode_t *rhs_;
        INode_t *lhs_;
        ConditionOp op_;

    public:
        return_type_t calc() override;

        NodeConditionOp_t(INode_t *lhs, ConditionOp op, INode_t *rhs = nullptr)
            : lhs_(lhs),
              op_(op),
              rhs_(rhs) {}
        ~NodeConditionOp_t() override
        {
            delete rhs_;
            delete lhs_;
        }
    };

    class NodeComp_t final : public INode_t
    {
    private:
        INode_t *lhs_;
        INode_t *rhs_;
        ComparingOp op_;

    public:
        return_type_t calc() override;

        NodeComp_t(INode_t *lhs, ComparingOp op, INode_t *rhs) : lhs_(lhs), op_(op), rhs_(rhs) {}
        ~NodeComp_t() override
        {
            delete lhs_;
            delete rhs_;
        }
    };

    class NodeIf_t final : public INode_t
    {
    private:
        INode_t *condition_;
        INode_t *scope_;

    public:
        return_type_t calc() override;

        NodeIf_t(INode_t *condition, INode_t *scope) : condition_(condition), scope_(scope) {}
    };

    class NodeWhile_t final : public INode_t
    {
    private:
        INode_t *condition_;
        INode_t *scope_;

    public:
        return_type_t calc() override;

        NodeWhile_t(INode_t *condition, INode_t *scope) : condition_(condition), scope_(scope) {}
    };

} // namespace INode