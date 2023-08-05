#include <sigil-ast/syntaxTree.h>

#include <stdexcept>

namespace sigil {

class TypeError : public std::exception
{
public:
    const std::string msg;

    TypeError(const std::string& msg)
        : msg(msg)
    {
    }

    virtual const char* what() const noexcept override
    {
        return msg.c_str();
    }
};

void typeCheck(ASTNodePtr tree);

} //namespace sigil