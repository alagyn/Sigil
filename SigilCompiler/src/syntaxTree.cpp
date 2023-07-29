#include <sigil/syntaxTree.h>

namespace sigil {

ExprNode::ExprNode()
    : type(ExprType::Error)
    , left(nullptr)
    , right(nullptr)
    , int_val(0)
    , float_val(0.0)
    , str_val("")
{
}

ExprNode::ExprNode(ExprType type, ASTNodePtr left, ASTNodePtr right)
    : type(type)
    , left(left)
    , right(right)
{
}

ExprNode::ExprNode(std::string str_val)
    : type(ExprType::Lit_Str)
    , str_val(str_val)
{
}

ExprNode::ExprNode(int int_val)
    : type(ExprType::Lit_Int)
    , int_val(int_val)
{
}

ExprNode::ExprNode(double float_val)
    : type(ExprType::Lit_Float)
    , float_val(float_val)
{
}

DataTypeNode::DataTypeNode(
    PrimitiveType type,
    ASTNodePtr subtype1,
    ASTNodePtr subtype2
)
    : type(type)
    , subtype1(subtype1)
    , subtype2(subtype2)
{
}

DataTypeNode::DataTypeNode(std::string name)
    : type(PrimitiveType::User)
    , name(name)
{
}

AccessModNode::AccessModNode(AccessModifier access)
    : access(access)
{
}

} //namespace sigil