#include <inc/ParseTable.h>
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

        while(true)
        {
            ParseToken token = scanner->nextToken();
            ParseAction nextAction =
                PARSE_TABLE[stack.back()][static_cast<unsigned>(token.symbol)];

            std::cout << SYMBOL_LOOKUP.at(static_cast<unsigned>(token.symbol))
                      << " " << token.lineNum << "." << token.charNum << " "
                      << token.text << " ";

            switch(nextAction.action)
            {
            case A:
                // TODO
                return std::shared_ptr<SyntaxTree>(nullptr);
            case S:
            {
                std::cout << "S" << nextAction.state << "\n";
                stack.push_back(nextAction.state);
                break;
            }
            case R:
            {
                auto reduction = REDUCTIONS[nextAction.state];
                std::cout << "Reduce to \""
                          << SYMBOL_LOOKUP[reduction.nontermID] << "\""
                          << " via rule: " << nextAction.state << " {"
                          << " pops: " << reduction.numPops
                          << " goto idx: " << reduction.nontermID << " }\n";
                for(int i = 0; i < reduction.numPops; ++i)
                {
                    stack.pop_back();
                }
                ParseAction nextGoto =
                    PARSE_TABLE[stack.back()][reduction.nontermID];

                if(nextGoto.action == A)
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
                for(int i = 0; i < TABLE_COLS; ++i)
                {
                    auto& x = PARSE_TABLE[stack.back()][i];
                    if(x.action != E)
                    {
                        ss << SYMBOL_LOOKUP[i] << " ";
                    }
                }

                ss << "\n";

                throw std::runtime_error(ss.str());
            }
            }
        }
    }

} //namespace sigil