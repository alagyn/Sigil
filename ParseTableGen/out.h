#pragma once
/*******
This file was generated by ParseTableGen, do not edit
*******/
#include <vector>
#include <string>

namespace Sigil {

enum class Action {
    Error,
    Shift,
    Reduce,
    Goto,
    Accept
};

namespace Terminal {
    constexpr char semicolon[] = ";";
    constexpr char open_curly[] = "{";
    constexpr char close_curly[] = "}";
    constexpr char open_paren[] = "(";
    constexpr char close_paren[] = ")";
    constexpr char equals_sign[] = "=";
    constexpr char pound[] = "#";
    constexpr char name[] = "[a-zA-Z][a-zA-Z0-9_]*";
    constexpr char integer[] = "[1-9][0-9]*";
}

const std::vector<std::string> terminals = {
    Terminal::semicolon,
    Terminal::open_curly,
    Terminal::close_curly,
    Terminal::open_paren,
    Terminal::close_paren,
    Terminal::equals_sign,
    Terminal::pound,
    Terminal::name,
    Terminal::integer
};

class ParseAction {
public:
    const Action action;
    const int state;

ParseAction(Action action, int state)
    : action(action)
    , state(state)
    {}
};

const ParseAction PARSE_TABLE[9][12] = {
{ ParseAction(Action::Error, 0), ParseAction(Action::Goto, 1), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 3), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 2), ParseAction(Action::Error, 0), ParseAction(Action::Accept, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Reduce, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 4), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 5), ParseAction(Action::Error, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 6), ParseAction(Action::Error, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 7), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Shift, 8), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Reduce, 0) },
{ ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Error, 0), ParseAction(Action::Reduce, 0) }
};
}
