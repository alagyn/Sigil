#include <sigil-ast/syntaxTree.h>

#include <imgui.h>

#include <memory>
#include <set>
#include <utility>
#include <vector>

namespace sigil {

class NodeWrap
{
public:
    ASTNodePtr base;
    int loc;
    ImVec2 pos;

    int nextId;

    NodeWrap(ASTNodePtr base, int& portIdGen)
        : base(base)
        , nextId(--portIdGen)
    {
    }

    void _render();

protected:
    virtual void render() = 0;
    virtual void links() = 0;
};

typedef std::shared_ptr<NodeWrap> NodeWrapPtr;

class ASTGraphBrowser
{
public:
    ASTGraphBrowser(ASTNodePtr tree);

    int run();

private:
    void render();
    int recurseLoadTree(ASTNodePtr tree, int depth, int loc);
    int maybeLoad(ASTNodePtr tree, int depth, int loc);

    void renderDef(DefNodePtr defNode);
    void renderDatatype(DatatypeNodePtr datatype);
    void renderStatement(StmtNodePtr stmt);
    void renderExpr(ExprNodePtr expr);

    ASTNodePtr tree;

    int portIdGen;

    std::vector<NodeWrapPtr> nodes;

    template<class T, class N>
    std::shared_ptr<T> makeWrap(N node, int depth, int parentLoc);
};

} //namespace sigil