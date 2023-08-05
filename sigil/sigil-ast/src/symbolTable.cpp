#include <sigil-ast/symbolTable.h>

#include <stdexcept>

namespace sigil {

const char* const storageClassName(StorageClass storage)
{
    switch(storage)
    {
    default:
        throw std::runtime_error("Invalid storage type");
    case StorageClass::Error:
        return "Error";
    case StorageClass::Local:
        return "Local";
    case StorageClass::Param:
        return "Param";
    case StorageClass::Global:
        return "Global";
    }
}

Symbol::Symbol(
    StorageClass storage,
    DatatypeNodePtr datatype,
    const std::string& name
)
    : storage(storage)
    , datatype(datatype)
    , name(name)
{
}

Scope::Scope()
{
}

void Scope::bind(SymbolPtr symbol)
{
    // We won't error check here since
    // we should be doing a lookup first
    symbols[symbol->name] = symbol;
}

SymbolPtr Scope::lookup(const std::string& name)
{
    // this will return nullptr automatically
    return symbols[name];
}

SymbolTable::SymbolTable()
{
    // Add the global scope
    scopes.emplace_back();
}

void SymbolTable::scopeEnter()
{
    // Add a new scope
    scopes.emplace_back();
}

void SymbolTable::scopeExit()
{
    // We won't error check here since we should be
    // checking depth first
    scopes.pop_back();
}

int SymbolTable::depth()
{
    return scopes.size();
}

void SymbolTable::bind(SymbolPtr symbol)
{
    scopes.back().bind(symbol);
}

SymbolPtr SymbolTable::lookup(const std::string& name)
{
    SymbolPtr out;
    // iterate backwards
    for(auto iter = scopes.rbegin(); iter != scopes.rend(); ++iter)
    {
        out = iter->lookup(name);
        if(out)
        {
            break;
        }
    }

    return out;
}

SymbolPtr SymbolTable::lookupLocal(const std::string& name)
{
    return scopes.back().lookup(name);
}

} //namespace sigil