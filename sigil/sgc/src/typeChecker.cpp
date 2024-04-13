#include <sgc/typeChecker.h>
#include <sigil-ast/symbolTable.h>

using namespace std;

namespace sigil {

bool recurseTypeCheck(ASTNodePtr tree, SymbolTable& table);

void typecheck(ASTNodePtr tree)
{
    SymbolTable table;
    recurseTypeCheck(tree, table);
}

bool checkDef(DefNodePtr defNode, SymbolTable& table, int& scopeIndex);
DatatypeNodePtr resolveExpr(ExprNodePtr expr, SymbolTable& table);
bool typeEquals(DatatypeNodePtr a, DatatypeNodePtr b);

bool recurseTypeCheck(ASTNodePtr tree, SymbolTable& table)
{
    while(tree)
    {
        switch(tree->nodeType)
        {
        case ASTNodeType::Definition:
        {
            checkDef(std::static_pointer_cast<DefNode>(tree), table);
            break;
        }
        case ASTNodeType::Statement:
        {
            break;
        }
        }

        tree = tree->next;
    }
}

bool checkDef(DefNodePtr defNode, SymbolTable& table)
{
    StorageClass sc =
        table.depth() == 1 ? StorageClass::Global : StorageClass::Local;

    if(table.lookupLocal(defNode->name))
    {
        // TODO error here, name redefinition
    }

    if(table.lookup(defNode->name))
    {
        // TODO warn here, name redefinition
    }

    auto symbol = make_shared<Symbol>(sc, defNode->datatype, defNode->name);

    if(defNode->value)
    {
        DatatypeNodePtr exprType = resolveExpr(defNode->value, table);
    }
}

DatatypeNodePtr resolveExpr(ExprNodePtr expr, SymbolTable& table)
{
    if(!expr)
    {
        return DatatypeNodePtr();
    }

    DatatypeNodePtr leftType = resolveExpr(expr->left, table);
    DatatypeNodePtr rightType = resolveExpr(expr->right, table);

    // TODO need to figure out numeric conversions
    // TODO need to figure out operator overloads

    switch(expr->type)
    {
    case ExprType::Add:
    case ExprType::Sub:
    {
        if(typeEquals(leftType, rightType))
        {
            return leftType;
        }
    }

    // Boolean expressions
    case ExprType::CompLT:
    case ExprType::CompLTEQ:
    case ExprType::CompGT:
    case ExprType::CompGTEQ:
    case ExprType::CompEQ:
    case ExprType::CompNEQ:
    case ExprType::In:
    case ExprType::NotIn:
    case ExprType::LogAnd:
    case ExprType::LogOr:
    case ExprType::LogNot:
    case ExprType::LitTrue:
    case ExprType::LitFalse:
        return make_shared<DatatypeNode>(PrimitiveType::Bool);
    }
}

/*
    This only checks equivalence, not if they are compatible
*/
bool typeEquals(DatatypeNodePtr a, DatatypeNodePtr b)
{
    // check for nulls
    if((!a && b) || (a && !b))
    {
        return false;
    }

    if(!a && !b)
    {
        return true;
    }

    // Check if they have the same primitive types
    if(a->type == b->type)
    {
        switch(a->type)
        {
        case PrimitiveType::Error:
            // TODO?
            return false;
        case PrimitiveType::Unknown:
            // TODO
            return false;

        case PrimitiveType::Array:
        case PrimitiveType::List:
        case PrimitiveType::Set:
            return typeEquals(a->subtype1, b->subtype1);

        case PrimitiveType::Map:
            return typeEquals(a->subtype1, b->subtype1)
                   && typeEquals(a->subtype2, b->subtype2);

        case PrimitiveType::Func:
        {
            if(!typeEquals(a->subtype1, b->subtype1))
            {
                return false;
            }

            DatatypeNodePtr curA = a->subtype2, curB = b->subtype2;
            while(curA && curB)
            {
                if(!typeEquals(curA, curB))
                {
                    return false;
                }

                curA = static_pointer_cast<DatatypeNode>(curA->next);
                curB = static_pointer_cast<DatatypeNode>(curB->next);
            }
        }

        case PrimitiveType::Enum:
        case PrimitiveType::Class:
        {
            // TODO? This isn't doing name resolution
            // Need to do a lookup and get a fully qualified name
            return a->name == b->name;
        }
            // TODO constr/deleter?
            //

        default:
            // primitives with no subtypes, ints/floats/bool/etc
            return true;
        }
    }

    return false;
}

} //namespace sigil