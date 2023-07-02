#pragma once

#include <string>

namespace sigil {

    enum class ExprType
    {
        Add,
        Sub,
        Mul,
        Div,
        Pow,
        Not,
        Name,
        Lit_Int,
        Lit_Float,
        Lit_Str,
        Call,
        Subscript
    };

    class Expr
    {
    public:
        Expr();

        ExprType type;
        Expr* left;
        Expr* right;

        std::string name;
        int int_val;
        float float_val;
        std::string str_val;
    };

    class Statement
    {
    public:
        Statement();

    private:
        Statement* next;
    };

    class Declaration
    {
    public:
        Declaration();

        std::string name;
    };

    class SyntaxTree

    {
    public:
        SyntaxTree();
    };
} //namespace sigil