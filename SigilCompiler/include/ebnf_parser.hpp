#pragma once

#include <string>
#include <memory>
#include <list>
#include "syntaxTree.hpp"

namespace sigil 
{
    using SyntaxTreePtr = std::shared_ptr<SyntaxTree>;

    class Parser
    {
    public:
        Parser(const std::string& descrFile);

        SyntaxTreePtr parse(const std::string& in);

    private:
        class Definition
        {
        public:
            const std::string nonterm;
            std::list<std::string> defin;

            Definition(const std::string& nonterm):
                nonterm(nonterm),
                defin()
            {}
            
            void operator+(const std::string& o);
        };

        // List of EBNF definitions
        std::list<Definition> defins;
    };
}