#include "sigil_errors.hpp"

#include <sstream>

using namespace sigil;
using namespace std;

string concat(const std::string& s1, int i)
{
    stringstream ss;
    ss << s1 << i;
    return ss.str();
}

SigilError::SigilError(const std::string& msg, const std::string& header)
{
    stringstream ss;
    ss << header << ": " << msg;
    this->msg = ss.str();
}

const char* SigilError::what() const throw()
{
    return msg.c_str();
}


EBNF_ParseError::EBNF_ParseError(int line, const std::string& msg):
    SigilError(msg, concat("EBNF Parser: Line ", line))
{
}

PythonError::PythonError(const string& msg):
    SigilError(msg, "Python")
{}