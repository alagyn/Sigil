#include <sgc/typeChecker.h>
#include <sigil-ast/symbolTable.h>

namespace sigil {

void typecheck(ASTNodePtr tree)
{
    SymbolTable table;
    recurseTypeCheck(tree, table);
}

void checkDef(DefNodePtr defNode, SymbolTable& table);

void recurseTypeCheck(ASTNodePtr tree, SymbolTable& table)
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
    }
}

void checkDef(DefNodePtr defNode, SymbolTable& table)
{
    StorageClass sc =
        table.depth() == 1 ? StorageClass::Global : StorageClass::Local;

    if(table.lookup(defNode->name))
    {
    }

    switch(defNode->defType)
    {
    case DefType::Var:
    {
        break;
    }
    case DefType::Import:
        break;
    }
}

} //namespace sigil