#pragma once

#include <memory>
#include <string>

namespace sigil {

enum class ASTNodeType
{
    Expr
};

class ASTNode
{
};

using ASTNodePtr = std::shared_ptr<ASTNode>;

enum class ExprType
{
    Error,
    Add,
    Sub,
    Mul,
    Div,
    Pow,
    Not,
    Neg,
    Name,
    Lit_Int,
    Lit_Float,
    Lit_Str,
    Call,
    Subscript
};

class ExprNode : public ASTNode
{
public:
    ExprNode();
    ExprNode(ExprType type, ASTNodePtr left, ASTNodePtr right);
    ExprNode(std::string str_val);
    explicit ExprNode(int int_val);
    explicit ExprNode(double float_val);

    ExprType type;
    ASTNodePtr left;
    ASTNodePtr right;

    int int_val;
    double float_val;
    std::string str_val;
};

enum class PrimitiveType
{
    Error,
    Int,
    Float,
    Str,
    Array,
    List,
    Map,
    Set,
    User
};

class DataTypeNode : public ASTNode
{
public:
    DataTypeNode(
        PrimitiveType type,
        ASTNodePtr subtype1 = nullptr,
        ASTNodePtr subtype2 = nullptr
    );
    DataTypeNode(std::string name);

    PrimitiveType type;
    ASTNodePtr subtype1;
    // Subtype2 only used for maps?
    ASTNodePtr subtype2;
    std::string name;
};

enum class AccessModifier
{
    Public,
    Private,
    Readonly,
    Shared
};

class AccessModNode : public ASTNode
{
public:
    AccessModNode(AccessModifier access);

    AccessModifier access;
};

class Statement
{
public:
    Statement();

private:
    Statement* next;
};

class DefinitionNode : public ASTNode
{
public:
    DefinitionNode();

    std::string name;

    ASTNodePtr next;
};

} //namespace sigil