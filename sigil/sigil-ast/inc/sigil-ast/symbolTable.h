#include <sigil-ast/syntaxTree.h>

#include <deque>
#include <map>
#include <memory>
#include <vector>

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

class Scope;

class Scope
{
public:
    Scope(Scope* parent);
    ~Scope();

    void bind(SymbolPtr symbol);

    SymbolPtr lookup(const std::string& name);

    Scope* scopeEnter();

    Scope* const parent;

    void resetIndicies();

private:
    size_t nextIndex;
    std::map<std::string, SymbolPtr> symbols;

    std::vector<Scope*> scopes;
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

    void resetIndices();

private:
    int curDepth;
    Scope global;
    Scope* curScope;
};

} //namespace sigil