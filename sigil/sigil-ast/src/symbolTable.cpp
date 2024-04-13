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

Scope::Scope(Scope* parent)
    : parent(parent)
{
}

Scope::~Scope()
{
    for(auto s : scopes)
    {
        delete s;
    }
}

Scope* Scope::scopeEnter()
{
    Scope* out;
    if(nextIndex < scopes.size())
    {
        out = scopes[nextIndex];
    }
    else
    {
        auto out = new Scope(this);
        scopes.push_back(out);
    }

    ++nextIndex;
    return out;
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

void Scope::resetIndicies()
{
    nextIndex = 0;
    for(auto scope : scopes)
    {
        scope->resetIndicies();
    }
}

SymbolTable::SymbolTable()
    : curDepth(1)
    , global(nullptr)
    , curScope(&global)
{
}

void SymbolTable::scopeEnter()
{
    curScope = curScope->scopeEnter();
    ++curDepth;
}

void SymbolTable::scopeExit()
{
    curScope = curScope->parent;
    --curDepth;
}

int SymbolTable::depth()
{
    return curDepth;
}

void SymbolTable::bind(SymbolPtr symbol)
{
    curScope->bind(symbol);
}

SymbolPtr SymbolTable::lookup(const std::string& name)
{
    SymbolPtr out;
    Scope* cur = curScope;
    while(cur != nullptr)
    {
        out = cur->lookup(name);
        if(out)
        {
            break;
        }
        cur = cur->parent;
    }

    return out;
}

SymbolPtr SymbolTable::lookupLocal(const std::string& name)
{
    return curScope->lookup(name);
}

void SymbolTable::resetIndices()
{
    global.resetIndicies();
}

} //namespace sigil