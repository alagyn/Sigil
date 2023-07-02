#pragma once

#include <fstream>
#include <string>

namespace sigil {

    enum class Symbol;

    typedef struct
    {
        Symbol symbol;
        std::string text;
        unsigned lineNum;
        unsigned charNum;
    } ParseToken;

    class Scanner

    {
    public:
        Scanner(std::string filename);
        ~Scanner();

        ParseToken nextToken();

    private:
        std::fstream handle;

        int curLineNum;
        int curCharNum;
    };

} //namespace sigil