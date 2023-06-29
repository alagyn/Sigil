#pragma once

#include <fstream>
#include <string>

#include <inc/ParseTable.h>

namespace sigil {

    class Scanner
    {
    public:
        Scanner(std::string filename);
        ~Scanner();

        Symbol nextToken(std::string& out);

    private:
        std::fstream handle;

        int curLineNum;
        int curCharNum;
    };

} //namespace sigil