#pragma once

namespace INode
{
    enum class MathOp
    {
        Nothing = 0,
        Plus,
        Minus,
        Div,
        Mult,

    };

    enum class ConditionOp
    {
        Nothing = 0,
        And,
        Or,
        Not
    };

    enum class ComparingOp
    {
        Nothing = 0,
        BiggerEqual,
        LessEqual,
        Equal,
        Bigger,
        Less,
        NotEqual
    };

    enum class CommonOp
    {
        Nothing = 0,
        Assign,
        Output,
        Input
    };

    struct INode_t
    {
        using return_type_t = double;

        virtual return_type_t calc() = 0;
        virtual ~INode_t() {}
    };

    struct IScope_t : INode_t
    {
        virtual void AddBranch(INode_t *branch) = 0;
        virtual IScope_t *Duplicate() = 0;
        virtual IScope_t *Reset() const = 0;
        virtual INode_t *AddName(const std::string &name) = 0;
        virtual INode_t *GetName(const std::string &name) = 0;
    };

    INode_t *CreateDouble(double val);
    INode_t *CreateName();

    INode_t *CreateMathOp(
        INode_t *lhs, MathOp op, INode_t *rhs);
    INode_t *CreateConditionOp(
        INode_t *lhs, ConditionOp op, INode_t *rhs = nullptr);
    INode_t *CreateCompOp(
        INode_t *lhs, ComparingOp op, INode_t *rhs);
    INode_t *CreateCommonOp(
        INode_t *lhs, CommonOp op, INode_t *rhs = nullptr);

    INode_t *CreateIf(
        INode_t *cond, INode_t *scope);
    INode_t *CreateWhile(
        INode_t *cond, INode_t *scope);

    IScope_t *CreateScope();

    ComparingOp DefineCompType(const std::string &token);
} // namespace INode