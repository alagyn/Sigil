#pragma once

#include <fstream>
#include <string>

namespace sigil {
namespace parseTable {
    enum class Symbol;
}

typedef struct
{
    parseTable::Symbol symbol;
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

    void consumeNewLine(char& nextChar);
};

} //namespace sigil