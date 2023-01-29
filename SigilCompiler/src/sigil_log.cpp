#include "sigil_log.hpp"

#include <iostream>

const char C_ERR[] = "\033[91m";
const char C_DBG[] = "\033[92m";
const char C_END[] = "\033[0m";

using namespace sigil;
using namespace std;

// TODO log levels?

void sigil::logInfo(const std::string& msg)
{
    cout << "INFO :" << msg << endl;
}

void sigil::logDbg(const std::string& msg)
{
    cout << C_DBG << "DEBUG:" << msg << C_END << endl;
}

void sigil::logErr(const std::string& msg)
{
    cout << C_ERR << "ERROR:" << msg << C_END << endl;
}

const char* sigil::getErr()
{
    return C_ERR;
}

const char* sigil::getClr()
{
    return C_END;
}