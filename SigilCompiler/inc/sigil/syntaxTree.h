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
    SpecialMod,
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
    // Used when we get a user defined type
    Unknown,
    Int,
    Float,
    Str,
    Array,
    List,
    Map,
    Set,
    Func,
    Class,
    Enum
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
    // Normal types, the map key type, or the return type
    ASTNodePtr subtype1;
    // map value type, or the function arg list
    ASTNodePtr subtype2;
    std::string name;
};

enum class AccessMod
{
    Default,
    Public,
    Private,
    Readonly,
    Shared
};

class AccessModNode : public ASTNode
{
public:
    AccessModNode(AccessMod access);

    AccessMod accessMod;
};

enum class StmtType
{
    Assign,
    If,
    ForIn,
    ForTo,
    While,
    DoWhile,
    Return
};

class StmtNode : public ASTNode
{
public:
    StmtNode(StmtType stmtType);

    StmtType stmtType;
    // Declarations for "for" and "while" loops, and types for assignment
    ASTNodePtr decl;
    // Expressions for "for" and "while" continuation, and "if" checks
    ASTNodePtr check;
    // Second expression in "ForTo", and names for assignments
    ASTNodePtr update;
    // Code body
    ASTNodePtr body;
    // else statements, and elses for ternary
    ASTNodePtr elseStmt;
};

enum class SpecialMod
{
    None,
    Abstract,
    Static
};

class SpecialModNode : public ASTNode
{
public:
    SpecialModNode(SpecialMod specialMod);

    SpecialMod specialMod;
};

enum class DefType
{
    Error,

    // Body is the epxression used to assign. Works
    // the same for func args
    Var,

    // Funcs use the parent type for the special mod type
    // And body as the arg definitions
    Func,

    Class,
    Enum
};

class DefNode : public ASTNode
{
public:
    DefNode(DefType defType, ASTNodePtr dataType, std::string name);

    const DefType defType;
    const ASTNodePtr dataType;
    const std::string name;

    AccessMod accessMod;
    SpecialMod specialMod;
    ASTNodePtr body;
};

} //namespace sigil