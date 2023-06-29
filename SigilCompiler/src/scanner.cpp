#include <inc/scanner.h>

#include <iostream>
#include <list>
#include <sstream>

#include <boost/regex.h>

using namespace std;

namespace sigil {
    Scanner::Scanner(string filename)
        : handle(filename)
        , curLineNum(0)
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

    Symbol Scanner::nextToken(std::string& out)
    {
        if(handle.eof())
        {
            return Symbol::__EOF__;
        }

        out.clear();

        // Initialize set of available terminals
        list<const Terminal*> availableTerminals;

        for(const Terminal& t : TERMINALS)
        {
            availableTerminals.push_back(&t);
        }

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
                    return availableTerminals.front()->id;
                }
                else
                {
                    break;
                }
            }
            ++curCharNum;

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
                out.push_back(nextChar);
            }

            /*
                Copy the list
                We need to do this so that if the current char
                causes all available matches to be pruned we can still
                see what was available and take the first option
            */
            list<const Terminal*> prunedList = availableTerminals;

            auto iter = prunedList.begin();
            // Manually control iteration for deletes
            while(iter != prunedList.end())
            {
                const Terminal* t = *iter;

                if(boost::regex_match(out, t->re))
                {
                    foundMatch = true;
                    ++iter;
                }
                // If it doesn't match and we have gotten a match before, prune it
                else if(foundMatch)
                {
                    auto curIter = iter;
                    ++iter;
                    // Remove this from the set
                    prunedList.erase(curIter);
                }
                else
                {
                    // Else just go to the next terminal
                    ++iter;
                }
            }

            if(nextChar == '\n')
            {
                ++curLineNum;
                curCharNum = 0;
            }

            if(foundMatch)
            {
                // The currentChar pruned every option
                if(prunedList.empty())
                {
                    // Therefore we have found the maximal-munch
                    // We need to unget the last char so we don't consume it
                    if(!hitWhitespace)
                    {
                        handle.unget();
                        out.pop_back();
                    }

                    /*
                        If we happen to prune everything in one go, we cannot
                       assume that every terminal in the prior set was valid in
                       the first place therefore check each again and take the
                       first that matches
                    */
                    for(auto term : availableTerminals)
                    {
                        if(boost::regex_match(out, term->re))
                        {
                            return term->id;
                        }
                    }

                    // If we got here something bad happened and nothing matched
                    std::stringstream ss;
                    ss << "Bad token: '" << out << "'";
                    throw std::runtime_error(ss.str());
                }
                // Or we hit whitespace but we didn't prune everything
                if(hitWhitespace)
                {
                    // Therefore take the first match in the pruned list
                    return prunedList.front()->id;
                }
            }

            //Else, we haven't matched anything yet, continue...
            // TODO have a max char limit?

            // Only copy if we have found a match and we pruned something
            if(foundMatch && availableTerminals.size() != prunedList.size())
            {
                availableTerminals = prunedList;
            }
        }

        // We only got here if there was an EOF
        // Return EOF which will error later?
        // TODO make sure this is the case?
        return Symbol::__EOF__;
    };
} //namespace sigil