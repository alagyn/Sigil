#pragma once

#include <memory>
#include <string>

namespace sigil {

enum class ASTNodeType
{
    Error,
    Expr,
    Statement,
    Datatype,
    AccessMod,
    Definition
};

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode
{
public:
    ASTNode(ASTNodeType nodeType);

    const ASTNodeType nodeType;
    ASTNodePtr next;
};

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

    LitInt,
    LitFloat,
    LitStr,

    // These have left set to an expr list, right == nullptr
    LitList,
    LitSet,

    // Left is key, right is val, next is next value
    LitMap,

    Call,
    Dot,
    Subscript
};

class ExprNode : public ASTNode
{
public:
    ExprNode();
    ExprNode(ExprType type, ASTNodePtr left, ASTNodePtr right);
    ExprNode(ExprType type, std::string name);
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

class Statement : public ASTNode
{
public:
    Statement();
};

class DefinitionNode : public ASTNode
{
public:
    DefinitionNode();

    std::string name;
};

} //namespace sigil