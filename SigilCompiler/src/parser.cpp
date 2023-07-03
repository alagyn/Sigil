#include <inc/parseTable.h>
#include <inc/parser.h>

#include <iostream>
#include <sstream>

namespace sigil {

Parser::Parser(std::shared_ptr<Scanner> scanner)
    : scanner(scanner)
{
}

std::shared_ptr<SyntaxTree> Parser::parse()
{
    // Init by pushing the starting state onto the stack
    stack.push_back(0);

    ParseToken token = scanner->nextToken();

    while(true)
    {
        parseTable::ParseAction nextAction =
            parseTable::getAction(stack.back(), token.symbol);

        std::cout << parseTable::symbolLookup(token.symbol) << " "
                  << token.lineNum << "." << token.charNum << " " << token.text
                  << " ";

        switch(nextAction.action)
        {
        case parseTable::A:
            // TODO
            return std::shared_ptr<SyntaxTree>(nullptr);
        case parseTable::S:
        {
            std::cout << "S" << nextAction.state << "\n";
            stack.push_back(nextAction.state);
            // We only get the next token after a shift
            token = scanner->nextToken();
            break;
        }
        case parseTable::R:
        {
            auto reduction = parseTable::getReduction(nextAction.state);
            std::cout << "Reduce to \""
                      << parseTable::symbolLookup(reduction.nonterm) << "\""
                      << " via rule: " << nextAction.state << " {"
                      << " pops: " << reduction.numPops << " goto idx: "
                      << static_cast<unsigned>(reduction.nonterm) << " }\n";
            for(int i = 0; i < reduction.numPops; ++i)
            {
                stack.pop_back();
            }
            parseTable::ParseAction nextGoto =
                parseTable::getAction(stack.back(), reduction.nonterm);

            if(nextGoto.action == parseTable::A)
            {
                // TODO accept
                return std::shared_ptr<SyntaxTree>(nullptr);
            }

            stack.push_back(nextGoto.state);

            break;
        }
        default:
        {
            std::cout << "\n";
            std::stringstream ss;
            ss << "Parse Error at line " << token.lineNum << " char "
               << token.charNum << " token: " << token.text << "\n";
            ss << "Stack: ";
            for(auto x : stack)
            {
                ss << x << " ";
            }

            ss << "Expected one of: ";
            for(int i = 0; i < parseTable::numCols(); ++i)
            {
                auto x = parseTable::getAction(stack.back(), i);
                if(x.action != parseTable::E)
                {
                    ss << parseTable::symbolLookup(i) << " ";
                }
            }

            ss << "\n";

            throw std::runtime_error(ss.str());
        }
        }
    }
}

} //namespace sigil