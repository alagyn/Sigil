#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <list>

#include "sigil_errors.hpp"
#include "ebnf_parser.hpp"
#include "sigil_log.hpp"

#include "Python.h"

using namespace sigil;
using namespace std;


const char PARSE_PY[] = "SPython.ebnf_parser";

Parser::Parser(const string& descrFile)
{
    Py_Initialize();

    PyObject* module = PyImport_ImportModule(PARSE_PY);

    if(module == NULL)
    {
        cout << getErr();
        PyErr_PrintEx(0);
        cout << getClr();

        stringstream ss;
        ss << "Cannot import module \"" << PARSE_PY << "\"";
        throw PythonError(ss.str());
    }

    PyObject* arg = PyUnicode_FromString(descrFile.c_str());
    PyObject* argTuple = PyTuple_New(1);
    PyTuple_SetItem(argTuple, 0, arg);

    PyObject* func = PyObject_GetAttrString(module, "parse");

    PyObject* out = PyObject_CallObject(func, argTuple);

    Py_Finalize();
}

SyntaxTreePtr Parser::parse(const string& in)
{
    //TODO
    return SyntaxTreePtr();
}

void Parser::Definition::operator+(const std::string &o)
{
    defin.emplace_back(o);
}
