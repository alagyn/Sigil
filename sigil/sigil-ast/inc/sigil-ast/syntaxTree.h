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

const char* const astNodeTypeName(ASTNodeType);

class ASTNode;
using ASTNodePtr = std::shared_ptr<ASTNode>;

class ASTNode
{
public:
    ASTNode(ASTNodeType nodeType);

    const ASTNodeType nodeType;
    const unsigned nodeID;
    ASTNodePtr next;
};

enum class ExprType
{
    Error,
    Add,
    Sub,
    Mod,
    Mul,
    Div,
    Pow,
    Not,
    Neg,
    Name,

    PreInc,
    PostInc,
    PreDec,
    PostDec,

    CompLT,
    CompLTEQ,
    CompGT,
    CompGTEQ,
    CompEQ,
    CompNEQ,

    LogAnd,
    LogOr,
    LogNot,

    BitAnd,
    BitOr,
    BitXOr,
    BitNot,

    LitNull,
    LitTrue,
    LitFalse,
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

const char* const exprTypeName(ExprType);

class ExprNode : public ASTNode
{
public:
    ExprNode();
    ExprNode(ExprType type, ASTNodePtr left, ASTNodePtr right);
    ExprNode(ExprType type, std::string name);
    ExprNode(ExprType type);
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

using ExprNodePtr = std::shared_ptr<ExprNode>;

enum class PrimitiveType
{
    Error,
    Null,
    // Used when we get a user defined type
    Unknown,
    Bool,
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

const char* const primitiveTypeName(PrimitiveType);

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

using DataTypeNodePtr = std::shared_ptr<DataTypeNode>;

enum class AccessMod
{
    Default,
    Public,
    Private,
    Readonly,
    Shared
};

const char* const accessModName(AccessMod);

class AccessModNode : public ASTNode
{
public:
    AccessModNode(AccessMod access);

    AccessMod accessMod;
};

enum class StmtType
{
    Error,

    Assign,
    AssignAdd,
    AssignSub,
    AssignMul,
    AssignDiv,
    AssignMod,

    AssignAnd,
    AssignOr,
    AssignXOr,

    If,
    ForIn,
    ForTo,
    While,
    DoWhile,
    Return,
    Throw,
    Try,
    Catch
};

const char* const stmtTypeName(StmtType);

class StmtNode : public ASTNode
{
public:
    StmtNode(StmtType stmtType);

    StmtType stmtType;
    // Declarations for "for" and "while" loops, and types for assignment
    ASTNodePtr decl;
    // Expressions for "for" and "while" continuation, and "if" checks,
    // datatypes for catches
    ASTNodePtr check;
    // Second expression in "ForTo", and names for assignments and catch vars
    ASTNodePtr update;
    // Code body, main code for trys
    ASTNodePtr body;
    // else statements, and elses for ternary, and catch for trys
    ASTNodePtr elseStmt;
};

using StmtNodePtr = std::shared_ptr<StmtNode>;

enum class SpecialMod
{
    None,
    Abstract,
    Static
};

const char* const specialModName(SpecialMod);

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
    // Contructor
    Init,
    Delete,

    Class,
    Enum
};

const char* const defTypeName(DefType);

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

using DefNodePtr = std::shared_ptr<DefNode>;

} //namespace sigil