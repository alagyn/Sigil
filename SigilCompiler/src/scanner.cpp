#include <inc/scanner.h>

#include <iostream>
#include <list>
#include <sstream>

#include <boost/regex.h>

#include <inc/ParseTable.h>

using namespace std;

namespace sigil {
    Scanner::Scanner(string filename)
        : handle(filename)
        , curLineNum(1)
        , curCharNum(0)
    {
    }

    Scanner::~Scanner()
    {
        if(handle.is_open())
        {
            handle.close();
        }
    }

    ParseToken Scanner::nextToken()
    {
        ParseToken out;
        out.lineNum = curLineNum;
        out.charNum = curCharNum;

        if(handle.eof())
        {
            out.symbol = Symbol::__EOF__;
            return out;
        }

        // TODO optimizations here
        /*
            we can maybe keep track of whether each terminal has starting
           matching yet and prune when they stop. This will cause errors if the
           re has to ability to go in and out of matching, but is that a common
           thing for the kinds of re here?
        */

        // flag for if we have started having matches
        bool foundMatch = false;

        while(!handle.eof())
        {
            char nextChar;
            handle.get(nextChar);
            if(handle.eof())
            {
                // If we are at EOF and we have found a match
                if(foundMatch)
                {
                    for(auto& x : TERMINALS)
                    {
                        if(boost::regex_match(out.text, x.re))
                        {
                            // Take the first that matches
                            out.symbol = x.id;
                        }
                    }
                    return out;
                }
                else
                {
                    break;
                }
            }
            ++curCharNum;
            ++out.charNum;

            // Handle windows/mac line endings
            if(nextChar == '\r')
            {
                handle.get(nextChar);
                if(nextChar != '\n')
                {
                    // If it wasn't actually \r\n, unget
                    handle.unget();
                    // Force to \n to for next if block
                    nextChar = '\n';
                }
            }

            bool hitWhitespace =
                nextChar == ' ' || nextChar == '\t' || nextChar == '\n';

            if(!hitWhitespace)
            {
                // Don't include whitespace in the char
                out.text.push_back(nextChar);
            }

            bool foundNewMatch = false;
            for(auto& t : TERMINALS)
            {
                if(boost::regex_match(out.text, t.re))
                {
                    foundNewMatch = true;
                    // Short circuit. We only need to see if a single one matches
                    break;
                }
            }

            if(nextChar == '\n')
            {
                ++curLineNum;
                ++out.lineNum;
                curCharNum = 0;
            }

            // If we haven't gotten a match yet and we just found a new one
            if(!foundMatch && foundNewMatch)
            {
                // Set foundMatch to true
                foundMatch = true;
            }
            // Else if we previously found a match and then stopped or we hit whitespace
            else if(foundMatch && (!foundNewMatch || hitWhitespace))
            {
                // Therefore we have found the maximal-munch
                if(!hitWhitespace)
                {
                    // We need to unget the last char so we don't consume it
                    // we already skip whitespace
                    handle.unget();
                    out.text.pop_back();
                }

                /*
                    Find the first terminal that matches
                */
                for(auto& term : TERMINALS)
                {
                    if(boost::regex_match(out.text, term.re))
                    {
                        out.symbol = term.id;
                        return out;
                    }
                }

                // If we got here something bad happened and nothing matched
                std::stringstream ss;
                ss << "Bad token: '" << out.text << "'";
                throw std::runtime_error(ss.str());
            }
        }

        // We only got here if there was an EOF
        // Return EOF which will error later?
        // TODO make sure this is the case?
        out.symbol = Symbol::__EOF__;
        return out;
    };
} //namespace sigil