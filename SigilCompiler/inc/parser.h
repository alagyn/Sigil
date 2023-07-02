#pragma once

#include <deque>
#include <memory>

#include <inc/scanner.h>
#include <inc/syntaxTree.h>

namespace sigil {

    class Parser
    {
    public:
        Parser(std::shared_ptr<Scanner> scanner);

        std::shared_ptr<SyntaxTree> parse();

    private:
        std::shared_ptr<Scanner> scanner;

        std::deque<unsigned> stack;
    };

} //namespace sigil