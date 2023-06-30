# Sigil EBNF syntax
All statements end with a semicolon.
New lines are allowed and do not affect the rules.
Comments start with a pound (#).

Terminals are of the form
[name] = "[regex]";
or
[name] = '[regex]';
Pounds are safe to use within quotes


Rules are of the form
[name] = [terminal/nonterminal]+ ... ;

Names follow the regex `[a-zA-Z0-9_]+` i.e. `\w`

Keywords:
* `EMPTY`: epsilon terminal, i.e. empty string.

The first rule is the starting rule for the grammer

Terminals are defined in order of precedence