#pragma once

#include <string>

namespace sigil
{
    void logInfo(const std::string& msg);
    void logDbg(const std::string& msg);
    void logErr(const std::string& msg);
    const char* getErr();
    const char* getClr();
}


