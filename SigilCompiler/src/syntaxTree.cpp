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

} //namespace sigil