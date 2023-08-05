#include <slog/slog.h>

#include <iostream>

namespace sigil {

const char C_ERR[] = "\033[91m";
const char C_DBG[] = "\033[92m";
const char C_END[] = "\033[0m";

void SLog::log(const std::string& msg)
{
    std::cout << msg << "\n";
}

} //namespace sigil