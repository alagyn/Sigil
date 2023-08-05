#include <sigil-ast/syntaxTree.h>

#include <deque>
#include <map>
#include <memory>

namespace sigil {

enum class StorageClass
{
    Error,
    Local,
    Param,
    Global
};

const char* const storageClassName(StorageClass storage);

class Symbol
{
public:
    StorageClass storage;
    DatatypeNodePtr datatype;
    std::string name;
    int index;

    Symbol(StorageClass storage, DatatypeNodePtr datatype, const std::string& name);
};

typedef std::shared_ptr<Symbol> SymbolPtr;

class Scope
{
public:
    Scope();

    void bind(SymbolPtr symbol);

    SymbolPtr lookup(const std::string& name);

private:
    std::map<std::string, SymbolPtr> symbols;
};

class SymbolTable
{
public:
    SymbolTable();

    // Enter a new scope
    void scopeEnter();
    // Exit a scope
    void scopeExit();
    // Get the current scope depth, depth 1 == global
    int depth();
    // Bind a symbol to the current table
    void bind(SymbolPtr symbol);
    // Find the first definition of the name, or nullptr
    SymbolPtr lookup(const std::string& name);
    // Try to find the definition of the name, but only check the current scope
    SymbolPtr lookupLocal(const std::string& name);

private:
    // using a deque so we can iterate over it
    std::deque<Scope> scopes;
};

} //namespace sigil