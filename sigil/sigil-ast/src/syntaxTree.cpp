#include <sigil-ast/syntaxTree.h>

#include <stdexcept>

namespace sigil {

unsigned NODE_IG_GEN = 0;

ASTNode::ASTNode(ASTNodeType nodeType)
    : nodeType(nodeType)
    , nodeID(NODE_IG_GEN++)
    , next(nullptr)
{
}

ExprNode::ExprNode()
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::Error)
    , left(nullptr)
    , right(nullptr)
    , int_val(0)
    , float_val(0.0)
    , str_val("")
{
}

ExprNode::ExprNode(ExprType type)
    : ASTNode(ASTNodeType::Expr)
    , type(type)
{
}

ExprNode::ExprNode(ExprType type, ASTNodePtr left, ASTNodePtr right)
    : ASTNode(ASTNodeType::Expr)
    , type(type)
    , left(left)
    , right(right)
{
}

ExprNode::ExprNode(ExprType type, std::string name)
    : ASTNode(ASTNodeType::Expr)
    , type(type)
    , str_val(name)
{
}

ExprNode::ExprNode(std::string str_val)
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::LitStr)
    , str_val(str_val.substr(1, str_val.size() - 2)) // strip off quotes
{
}

ExprNode::ExprNode(int64_t int_val)
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::LitInt)
    , int_val(int_val)
{
}

ExprNode::ExprNode(uint64_t uint_val)
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::LitUInt)
    , uint_val(uint_val)
{
}

ExprNode::ExprNode(double float_val)
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::LitFloat)
    , float_val(float_val)
{
}

DataTypeNode::DataTypeNode(
    PrimitiveType type,
    ASTNodePtr subtype1,
    ASTNodePtr subtype2
)
    : ASTNode(ASTNodeType::Datatype)
    , type(type)
    , subtype1(subtype1)
    , subtype2(subtype2)
{
}

DataTypeNode::DataTypeNode(std::string name)
    : ASTNode(ASTNodeType::Datatype)
    , type(PrimitiveType::Unknown)
    , name(name)
{
}

AccessModNode::AccessModNode(AccessMod access)
    : ASTNode(ASTNodeType::AccessMod)
    , accessMod(access)
{
}

SpecialModNode::SpecialModNode(SpecialMod specialMod)
    : ASTNode(ASTNodeType::SpecialMod)
    , specialMod(specialMod)
{
}

DefNode::DefNode(DefType defType, ASTNodePtr dataType, std::string name)
    : ASTNode(ASTNodeType::Definition)
    , defType(defType)
    , dataType(dataType)
    , name(name)
    , accessMod(AccessMod::Default)
    , specialMod(SpecialMod::None)
    , body(nullptr)
{
}

StmtNode::StmtNode(StmtType stmtType)
    : ASTNode(ASTNodeType::Statement)
    , stmtType(stmtType)
    , decl(nullptr)
    , check(nullptr)
    , update(nullptr)
    , body(nullptr)
    , elseStmt(nullptr)
{
}

const char* const astNodeTypeName(ASTNodeType type)
{
    switch(type)
    {
    default:
        throw std::runtime_error("Invalid type");
    case ASTNodeType::Error:
        return "Error";
    case ASTNodeType::Expr:
        return "Expression";
    case ASTNodeType::Statement:
        return "Statement";
    case ASTNodeType::Datatype:
        return "Datatype";
    case ASTNodeType::AccessMod:
        return "Access Modifier";
    case ASTNodeType::SpecialMod:
        return "Special Modifier";
    case ASTNodeType::Definition:
        return "Definition";
    }
}

const char* const exprTypeName(ExprType type)
{
    switch(type)
    {
    default:
        throw std::runtime_error("Invalid type");
    case ExprType::Error:
        return "Error";
    case ExprType::Add:
        return "Add";
    case ExprType::Sub:
        return "Sub";
    case ExprType::Mod:
        return "Mod";
    case ExprType::Mul:
        return "Mul";
    case ExprType::Div:
        return "Div";
    case ExprType::Pow:
        return "Pow";
    case ExprType::Not:
        return "Not";
    case ExprType::Neg:
        return "Neg";
    case ExprType::Name:
        return "Name";
    case ExprType::PreInc:
        return "PreInc";
    case ExprType::PostInc:
        return "PostInc";
    case ExprType::PreDec:
        return "PreDec";
    case ExprType::PostDec:
        return "PostDec";
    case ExprType::CompLT:
        return "CompLT";
    case ExprType::CompLTEQ:
        return "CompLTEQ";
    case ExprType::CompGT:
        return "CompGT";
    case ExprType::CompGTEQ:
        return "CompGTEQ";
    case ExprType::CompEQ:
        return "CompEQ";
    case ExprType::CompNEQ:
        return "CompNEQ";
    case ExprType::In:
        return "In";
    case ExprType::NotIn:
        return "NotIn";
    case ExprType::LogAnd:
        return "LogAnd";
    case ExprType::LogOr:
        return "LogOr";
    case ExprType::LogNot:
        return "LogNot";
    case ExprType::BitAnd:
        return "BitAnd";
    case ExprType::BitOr:
        return "BitOr";
    case ExprType::BitNot:
        return "BitNot";
    case ExprType::BitXOr:
        return "BitXOr";
    case ExprType::LShift:
        return "LShift";
    case ExprType::RShift:
        return "RShift";
    case ExprType::LitNull:
        return "LitNull";
    case ExprType::LitTrue:
        return "LitTrue";
    case ExprType::LitFalse:
        return "LitFalse";
    case ExprType::LitInt:
        return "LitInt";
    case ExprType::LitUInt:
        return "LitUInt";
    case ExprType::LitHex:
        return "LitHex";
    case ExprType::LitFloat:
        return "LotFloat";
    case ExprType::LitStr:
        return "LitStr";
    case ExprType::LitList:
        return "LitList";
    case ExprType::LitSet:
        return "LitSet";
    case ExprType::LitTuple:
        return "LitTuple";
    case ExprType::LitMap:
        return "LitMap";
    case ExprType::Call:
        return "Call";
    case ExprType::CallArgs:
        return "CallArgs";
    case ExprType::Dot:
        return "Dot";
    case ExprType::Subscript:
        return "Subscript";
    case ExprType::KeyVal:
        return "KeyVal";
    }
}

const char* const primitiveTypeName(PrimitiveType type)
{
    switch(type)
    {
    default:
        throw std::runtime_error("Invalid type");
    case PrimitiveType::Error:
        return "Error";
    case PrimitiveType::Null:
        return "Null";
    case PrimitiveType::Unknown:
        return "Unknown";
    case PrimitiveType::Bool:
        return "Bool";
    // Ints
    case PrimitiveType::Int8:
        return "Int8";
    case PrimitiveType::Int16:
        return "Int16";
    case PrimitiveType::Int32:
        return "Int32";
    case PrimitiveType::Int64:
        return "Int64";
    // Uints
    case PrimitiveType::UInt8:
        return "UInt8";
    case PrimitiveType::UInt16:
        return "UInt16";
    case PrimitiveType::UInt32:
        return "UInt32";
    case PrimitiveType::UInt64:
        return "UInt64";

    case PrimitiveType::Float:
        return "Float";
    case PrimitiveType::Str:
        return "Str";
    case PrimitiveType::Array:
        return "Array";
    case PrimitiveType::List:
        return "List";
    case PrimitiveType::Map:
        return "Map";
    case PrimitiveType::Set:
        return "Set";
    case PrimitiveType::Tuple:
        return "Tuple";
    case PrimitiveType::Func:
        return "Func";
    case PrimitiveType::Class:
        return "Class";
    case PrimitiveType::Enum:
        return "Enum";
    }
}

const char* const accessModName(AccessMod mod)
{
    switch(mod)
    {
    default:
        throw std::runtime_error("Invalid type");
    case AccessMod::Default:
        return "Default";
    case AccessMod::Public:
        return "Public";
    case AccessMod::Private:
        return "Private";
    case AccessMod::Readonly:
        return "Readonly";
    case AccessMod::Shared:
        return "Shared";
    }
}

const char* const stmtTypeName(StmtType type)
{
    switch(type)
    {
    default:
        throw std::runtime_error("Invalid type");
    case StmtType::Error:
        return "Error";
    case StmtType::Assign:
        return "Assign";
    case StmtType::AssignAdd:
        return "AssignAdd";
    case StmtType::AssignSub:
        return "AssignSub";
    case StmtType::AssignMul:
        return "AssignMul";
    case StmtType::AssignDiv:
        return "AssignDiv";
    case StmtType::AssignMod:
        return "AssignMod";
    case StmtType::AssignAnd:
        return "AssignAnd";
    case StmtType::AssignOr:
        return "AssignOr";
    case StmtType::AssignXOr:
        return "AssignXOr";
    case StmtType::If:
        return "If";
    case StmtType::ForIn:
        return "ForIn";
    case StmtType::ForTo:
        return "ForTo";
    case StmtType::ForWhile:
        return "ForWhile";
    case StmtType::While:
        return "While";
    case StmtType::DoWhile:
        return "DoWhile";
    case StmtType::With:
        return "With";
    case StmtType::Comprehension:
        return "Comprehension";
    case StmtType::Return:
        return "Return";
    case StmtType::Throw:
        return "Throw";
    case StmtType::Try:
        return "Try";
    case StmtType::Catch:
        return "Catch";
    }
}

const char* const specialModName(SpecialMod mod)
{
    switch(mod)
    {
    default:
        throw std::runtime_error("Invalid type");
    case SpecialMod::None:
        return "None";
    case SpecialMod::Abstract:
        return "Abstract";
    case SpecialMod::Static:
        return "Static";
    }
}

const char* const defTypeName(DefType type)
{
    switch(type)
    {
    default:
        throw std::runtime_error("Invalid type");
    case DefType::Error:
        return "Error";
    case DefType::Var:
        return "Var";
    case DefType::Func:
        return "Func";
    case DefType::Init:
        return "Constr";
    case DefType::Delete:
        return "Delete";
    case DefType::Class:
        return "Class";
    case DefType::Enum:
        return "Enum";
    case DefType::Import:
        return "Import";
    }
}

} //namespace sigil