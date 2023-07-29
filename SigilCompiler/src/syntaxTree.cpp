#include <sigil/syntaxTree.h>

namespace sigil {

ASTNode::ASTNode(ASTNodeType nodeType)
    : nodeType(nodeType)
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
    , str_val(str_val)
{
}

ExprNode::ExprNode(int int_val)
    : ASTNode(ASTNodeType::Expr)
    , type(ExprType::LitInt)
    , int_val(int_val)
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
    , type(PrimitiveType::User)
    , name(name)
{
}

AccessModNode::AccessModNode(AccessModifier access)
    : ASTNode(ASTNodeType::AccessMod)
    , access(access)
{
}

} //namespace sigil