#include <iostream>

#include "ebnf_parser.hpp"
#include "Python.h"

using namespace sigil;
using namespace std;

int main(int argc, char* argv[])
{
    if(argc != 2)
    {
        cout << "Invalid Arguments\n";
        return 1;
    }

    Parser parse(argv[1]);

    return 0;
}
