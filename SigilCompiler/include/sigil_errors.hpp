#pragma once

#include <exception>
#include <string>

namespace sigil
{
    class SigilError : public std::exception
    {

    private:
        std::string msg;
        
    public:
        SigilError(const std::string& msg, const std::string& header);

        virtual const char* what() const throw();
    };

    class EBNF_ParseError : public SigilError
    {
    public:
        EBNF_ParseError(int line, const std::string& msg);
    };

    class PythonError : public SigilError
    {
    public:
        PythonError(const std::string& msg);
    };

}