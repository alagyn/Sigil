#include <ast-browser/ui.h>

#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <imgui.h>

#include <iostream>
#include <sstream>

#include <sigil-ast/syntaxTree.h>

namespace sigil {

void GLFWErrCallback(int err, const char* msg)
{
    std::cout << "GLFW Error, code: " << err << " Msg: " << msg << "\n";
}

// Forward decl
void recurseRenderTree(ASTNodePtr tree);

constexpr ImGuiTableFlags tableFlags =
    ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_BordersInnerV;

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

void maybeRender(const std::string& label, const ASTNodePtr node)
{
    if(node)
    {
        if(ImGui::TreeNode(label.c_str()))
        {
            recurseRenderTree(node);
            ImGui::TreePop();
        }
    }
}

void renderDef(DefNodePtr defNode)
{
    if(ImGui::BeginTable("def", 2, tableFlags))
    {
        tableEntry("Type", defTypeName(defNode->defType));
        tableEntry("Name", defNode->name);
        tableEntry("Special", specialModName(defNode->specialMod));
        tableEntry("Access", accessModName(defNode->accessMod));

        ImGui::EndTable();

        recurseRenderTree(defNode->dataType);
        recurseRenderTree(defNode->body);
    }
}

void renderDatatype(DataTypeNodePtr datatype)
{
    if(ImGui::BeginTable("datatype", 2, tableFlags))
    {
        tableEntry("Name", datatype->name);
        tableEntry("Type", primitiveTypeName(datatype->type));

        ImGui::EndTable();

        maybeRender("SubType 1", datatype->subtype1);
        maybeRender("SubType 2", datatype->subtype2);
    }
}

void renderStatement(StmtNodePtr stmt)
{
    if(ImGui::BeginTable("stmt", 2, tableFlags))
    {
        tableEntry("Type", stmtTypeName(stmt->stmtType));

        ImGui::EndTable();

        maybeRender("Decl", stmt->decl);
        maybeRender("Check", stmt->check);
        maybeRender("Update", stmt->update);
        maybeRender("Body", stmt->body);
        maybeRender("Else", stmt->elseStmt);
    }
}

void renderExpr(ExprNodePtr expr)
{
    if(ImGui::BeginTable("expr", 2, tableFlags))
    {
        tableEntry("Type", exprTypeName(expr->type));
        switch(expr->type)
        {
        case ExprType::LitInt:
        {
            tableEntry("Int Val", "%d", expr->int_val);
            break;
        }
        case ExprType::LitFloat:
        {
            tableEntry("Float Val", "%3.4f", expr->float_val);
            break;
        }
        case ExprType::LitStr:
        {
            tableEntry("Str Val", "'%s'", expr->str_val.c_str());
            break;
        }
        case ExprType::Name:
        {
            tableEntry("Name", expr->str_val);
            break;
        }
        }

        ImGui::EndTable();

        maybeRender("Left", expr->left);
        maybeRender("Right", expr->right);
    }
}

void recurseRenderTree(ASTNodePtr tree)
{
    while(tree)
    {
        if(ImGui::TreeNode(
               tree.get(),
               "%d: %s",
               tree->nodeID,
               astNodeTypeName(tree->nodeType)
           ))
        {
            switch(tree->nodeType)
            {
            case ASTNodeType::Definition:
            {
                renderDef(std::static_pointer_cast<DefNode>(tree));
                break;
            }
            case ASTNodeType::Datatype:
            {
                renderDatatype(std::static_pointer_cast<DataTypeNode>(tree));
                break;
            }
            case ASTNodeType::Statement:
            {
                renderStatement(std::static_pointer_cast<StmtNode>(tree));
                break;
            }
            case ASTNodeType::Expr:
            {
                renderExpr(std::static_pointer_cast<ExprNode>(tree));
                break;
            }
            default:
                break;
            }

            ImGui::TreePop();
        }

        //recurseRenderTree(tree);

        tree = tree->next;
    }
}

void render(ASTNodePtr tree)
{
    if(ImGui::Begin("AST-Tree"))
    {
        recurseRenderTree(tree);
    }
    ImGui::End();
}

const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

int run(ASTNodePtr tree)
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

    while(true)
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        /////////////
        render(tree);
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

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}

} //namespace sigil