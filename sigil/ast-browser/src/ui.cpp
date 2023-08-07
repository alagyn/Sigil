#include <ast-browser/ui.h>

#include <imgui.h>

#include <imnodes.h>

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <iostream>
#include <map>
#include <sstream>

#include <sigil-ast/syntaxTree.h>

using namespace std;

namespace sigil {

void GLFWErrCallback(int err, const char* msg)
{
    std::cout << "GLFW Error, code: " << err << " Msg: " << msg << "\n";
}

void maybeAttr(int portID, ASTNodePtr node, const char* label)
{
    if(node)
    {
        ImNodes::BeginOutputAttribute(portID);
        ImGui::Text(label);
        ImNodes::EndOutputAttribute();
    }
}

void maybeLink(int portID, ASTNodePtr node)
{
    if(node)
    {
        ImNodes::Link(portID, portID, node->nodeID);
    }
}

void tableEntry(const char* label, const std::string& value)
{
    ImGui::TableNextColumn();
    ImGui::Text(label);
    ImGui::TableNextColumn();
    ImGui::Text(value.c_str());
}

template<typename... Args>
void tableEntry(const char* label, const char* fmt, Args... args)
{
    ImGui::TableNextColumn();
    ImGui::Text(label);
    ImGui::TableNextColumn();
    ImGui::Text(fmt, args...);
}

constexpr ImGuiTableFlags TABLE_FLAGS =
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;

void NodeWrap::_render()
{
    ImNodes::SetNodeGridSpacePos(base->nodeID, pos);
    ImNodes::BeginNode(base->nodeID);
    ImNodes::BeginNodeTitleBar();
    ImGui::Text(astNodeTypeName(base->nodeType));
    ImNodes::EndNodeTitleBar();

    ImNodes::BeginInputAttribute(base->nodeID);
    ImGui::Dummy(ImVec2(5, 5));
    ImNodes::EndInputAttribute();

    render();

    maybeAttr(nextId, base->next, "Next");

    ImNodes::EndNode();

    links();

    maybeLink(nextId, base->next);
}

class TempNodeWrap : public NodeWrap
{
public:
    ASTNodePtr node;

    TempNodeWrap(ASTNodePtr node, int& idGen)
        : NodeWrap(node, idGen)
        , node(node)
    {
    }

protected:
    void render() override
    {
    }

    void links() override
    {
    }
};

class DefNodeWrap : public NodeWrap

{
public:
    DefNodePtr node;

    int bodyPort;
    int dtPort;

    DefNodeWrap(DefNodePtr node, int& portIdGen)
        : NodeWrap(node, portIdGen)
        , node(node)
        , bodyPort(--portIdGen)
        , dtPort(--portIdGen)
    {
    }

protected:
    void render() override
    {
        if(ImGui::BeginTable("def", 2, TABLE_FLAGS))
        {
            tableEntry("Name", node->name);
            tableEntry("Special", specialModName(node->specialMod));
            tableEntry("Access", accessModName(node->accessMod));

            ImGui::EndTable();
        }

        maybeAttr(dtPort, node->datatype, "Data Type");
        maybeAttr(bodyPort, node->body, "Body");
    }

    void links() override
    {
        maybeLink(dtPort, node->datatype);
        maybeLink(bodyPort, node->body);
    }
};

class DataTypeNodeWrap : public NodeWrap
{
public:
    DatatypeNodePtr node;

    int sub1Port, sub2Port;

    DataTypeNodeWrap(DatatypeNodePtr node, int& idGen)
        : NodeWrap(node, idGen)
        , node(node)
        , sub1Port(--idGen)
        , sub2Port(--idGen)
    {
    }

protected:
    void render() override
    {
        if(ImGui::BeginTable("datatype", 2, TABLE_FLAGS))
        {
            tableEntry("Name", node->name);
            tableEntry("Type", primitiveTypeName(node->type));

            ImGui::EndTable();
        }

        maybeAttr(sub1Port, node->subtype1, "SubType 1");
        maybeAttr(sub2Port, node->subtype2, "SubType 2");
    }

    void links() override
    {
        maybeLink(sub1Port, node->subtype1);
        maybeLink(sub2Port, node->subtype2);
    }
};

class StmtNodeWrap : public NodeWrap
{
public:
    StmtNodePtr node;
    int decl, check, update, body, else_;

    StmtNodeWrap(StmtNodePtr node, int& idGen)
        : NodeWrap(node, idGen)
        , node(node)
        , decl(--idGen)
        , check(--idGen)
        , update(--idGen)
        , body(--idGen)
        , else_(--idGen)
    {
    }

protected:
    void render() override
    {
        if(ImGui::BeginTable("stmt", 2, TABLE_FLAGS))
        {
            tableEntry("Type", stmtTypeName(node->stmtType));

            ImGui::EndTable();

            maybeAttr(decl, node->decl, "Decl");
            maybeAttr(check, node->check, "Check");
            maybeAttr(update, node->update, "Update");
            maybeAttr(body, node->body, "Body");
            maybeAttr(else_, node->elseStmt, "Else");
        }
    }

    void links() override
    {
        maybeLink(decl, node->decl);
        maybeLink(check, node->check);
        maybeLink(update, node->update);
        maybeLink(body, node->body);
        maybeLink(else_, node->elseStmt);
    }
};

class ExprNodeWrap : public NodeWrap
{
public:
    ExprNodePtr node;
    int left, right;

    ExprNodeWrap(ExprNodePtr node, int& idGen)
        : NodeWrap(node, idGen)
        , node(node)
        , left(--idGen)
        , right(--idGen)
    {
    }

protected:
    void render() override
    {
        if(ImGui::BeginTable("expr", 2, TABLE_FLAGS))
        {
            tableEntry("Type", exprTypeName(node->type));
            switch(node->type)
            {
            case ExprType::LitInt:
            {
                tableEntry("Int Val", "%d", node->int_val);
                break;
            }
            case ExprType::LitFloat:
            {
                tableEntry("Float Val", "%3.4f", node->float_val);
                break;
            }
            case ExprType::LitStr:
            {
                tableEntry("Str Val", "'%s'", node->str_val.c_str());
                break;
            }
            case ExprType::Name:
            {
                tableEntry("Name", node->str_val);
                break;
            }
            }

            ImGui::EndTable();

            maybeAttr(left, node->left, "Left");
            maybeAttr(right, node->right, "Right");
        }
    }

    void links() override
    {
        maybeLink(left, node->left);
        maybeLink(right, node->right);
    }
};

ASTGraphBrowser::ASTGraphBrowser(ASTNodePtr tree)
    : tree(tree)
    , portIdGen(-1)
    , nodes()

{
    recurseLoadTree(tree, 0, 0);
}

constexpr int HOR_OFF = 300;
constexpr int VERT_OFF = 200;

template<class T, class N>
std::shared_ptr<T> ASTGraphBrowser::makeWrap(N node, int depth, int loc)
{
    auto out = make_shared<T>(node, portIdGen);
    out->loc = loc;
    out->pos.x = depth * HOR_OFF;
    out->pos.y = loc * VERT_OFF;

    return out;
}

int ASTGraphBrowser::maybeLoad(ASTNodePtr tree, int depth, int loc)
{
    if(tree)
    {
        int newLoc = recurseLoadTree(tree, depth, loc);
        if(newLoc == loc)
        {
            return newLoc + 1;
        }
        return newLoc;
    }
    return loc;
}

int ASTGraphBrowser::recurseLoadTree(ASTNodePtr tree, int depth, int loc)
{
    int nextDepth = depth + 1;

    while(tree)
    {
        int newLoc = loc;
        switch(tree->nodeType)
        {
        case ASTNodeType::Definition:
        {
            auto x = static_pointer_cast<DefNode>(tree);
            newLoc = maybeLoad(x->datatype, nextDepth, newLoc);
            newLoc = maybeLoad(x->body, nextDepth, newLoc);
            auto node = makeWrap<DefNodeWrap>(x, depth, loc);
            nodes.push_back(node);

            break;
        }
        case ASTNodeType::Datatype:
        {
            auto x = static_pointer_cast<DatatypeNode>(tree);
            newLoc = maybeLoad(x->subtype1, nextDepth, newLoc);
            newLoc = maybeLoad(x->subtype2, nextDepth, newLoc);
            auto node = makeWrap<DataTypeNodeWrap>(x, depth, loc);
            nodes.push_back(node);

            break;
        }
        case ASTNodeType::Statement:
        {
            auto x = static_pointer_cast<StmtNode>(tree);
            newLoc = maybeLoad(x->decl, nextDepth, newLoc);
            newLoc = maybeLoad(x->check, nextDepth, newLoc);
            newLoc = maybeLoad(x->update, nextDepth, newLoc);
            newLoc = maybeLoad(x->body, nextDepth, newLoc);
            newLoc = maybeLoad(x->elseStmt, nextDepth, newLoc);
            auto node = makeWrap<StmtNodeWrap>(x, depth, loc);
            nodes.push_back(node);

            break;
        }
        case ASTNodeType::Expr:
        {
            auto x = static_pointer_cast<ExprNode>(tree);
            newLoc = maybeLoad(x->left, nextDepth, newLoc);
            newLoc = maybeLoad(x->right, nextDepth, newLoc);
            auto node = makeWrap<ExprNodeWrap>(x, depth, loc);
            nodes.push_back(node);
            break;
        }
        default:
            nodes.push_back(makeWrap<TempNodeWrap>(tree, depth, loc));
            break;
        }

        //return recurseLoadTree(tree->next, nextDepth, newLoc);
        tree = tree->next;
        if(newLoc == loc)
        {
            loc = newLoc + 1;
        }
        else
        {
            loc = newLoc;
        }
    }
    return loc;
}

constexpr ImGuiWindowFlags windowFlags =
    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;

void ASTGraphBrowser::render()
{
    auto io = ImGui::GetIO();

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(io.DisplaySize));
    if(ImGui::Begin("AST-Tree", nullptr, windowFlags))
    {
        ImNodes::BeginNodeEditor();
        ImNodes::MiniMap();

        for(auto node : nodes)
        {
            node->_render();
        }

        ImNodes::EndNodeEditor();
    }
    ImGui::End();
}

const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int ASTGraphBrowser::run()
{
    glfwSetErrorCallback(GLFWErrCallback);
    if(!glfwInit())
    {
        std::cout << "Cannot init GLFW\n";
        return 1;
    }

    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);
    GLFWwindow* window =
        glfwCreateWindow(1024, 768, "AST-Browser", nullptr, nullptr);

    if(!window)
    {
        std::cout << "Cannot create window\n";
        return 1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImNodes::CreateContext();
    ImGui::StyleColorsDark();

    if(!ImGui_ImplGlfw_InitForOpenGL(window, true))
    {
        std::cout << "Cannot init GLFW for OpenGL\n";
        return 1;
    }

    if(!ImGui_ImplOpenGL3_Init(glsl_version))
    {
        std::cout << "Cannot init OpenGL\n";
        return 1;
    }

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);

    ImNodes::GetIO().EmulateThreeButtonMouse.Modifier = &ImGui::GetIO().KeyAlt;

    while(true)
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /////////////
        render();
        /////////////

        ImGui::Render();
        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);
        glViewport(0, 0, displayW, displayH);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

        if(glfwWindowShouldClose(window))
        {
            break;
        }
    }

    ImNodes::DestroyContext();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

} //namespace sigil