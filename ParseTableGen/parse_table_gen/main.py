import re
from typing import List
from argparse import ArgumentParser

import parse_table_gen.preprocessing as preprocess
from parse_table_gen.ebnf_grammer import parse_grammer, Grammer
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.lalr1_automata import LALR1Automata, ParseTable


def parse_ebnf_file(filename: str) -> Grammer:
    cleanLines = preprocess.read_and_clean(filename)
    combinedLines = preprocess.combine_lines(cleanLines)
    del cleanLines

    return parse_grammer(combinedLines)


def main():
    parser = ArgumentParser()
    parser.add_argument("grammer_file")
    parser.add_argument("output_file")

    args = parser.parse_args()
    grammer = parse_ebnf_file(args.grammer_file)
    ff = FirstAndFollow(grammer)
    lalr = LALR1Automata(grammer, ff)
    table = ParseTable(lalr)

    with open(args.output_file, mode='w') as f:
        f.write(
            "#pragma once\n"
            "/*******\nThis file was generated by ParseTableGen, do not edit\n*******/\n"
            "#include <vector>\n"
            "#include <string>\n"
            "\n"
            "namespace Sigil {\n"
            "\n"
            "enum class Action {\n"
            "    Error,\n"
            "    Shift,\n"
            "    Reduce,\n"
            "    Goto,\n"
            "    Accept\n"
            "};\n"
            "\n"
            'namespace Terminal {\n'
        )

        for terminal in grammer.terminalList:
            regex = re.sub(r"\\", r"\\\\", terminal[1])
            f.write(f'    constexpr char {terminal[0]}[] = "{regex}";\n')
        f.write("}\n\n")  # End namespace Terminal

        f.write("const std::vector<std::string> terminals = {\n")
        for idx, terminal in enumerate(grammer.terminalList):
            f.write(f"    Terminal::{terminal[0]}")
            if idx < len(grammer.terminalList) - 1:
                f.write(",")
            f.write("\n")
        f.write("};\n\n")  # End terminal list

        f.write(
            "class ParseAction {\n"
            "public:\n"
            "    const Action action;\n"
            "    const int state;\n"
            "\n"
            "ParseAction(Action action, int state)\n"
            "    : action(action)\n"
            "    , state(state)\n"
            "    {}\n"
            "};\n"
            "\n"
            f"const ParseAction PARSE_TABLE[{len(table.table)}][{len(table.table[0])}] = {{\n"
        )

        for rowIdx, row in enumerate(table.table):
            f.write("{ ")
            for idx, action in enumerate(row):
                if action[0] == ParseTable.ACCEPT:
                    actEnum = "Accept"
                elif action[0] == ParseTable.GOTO:
                    actEnum = "Goto"
                elif action[0] == ParseTable.REDUCE:
                    actEnum = "Reduce"
                elif action[0] == ParseTable.SHIFT:
                    actEnum = "Shift"
                else:
                    actEnum = "Error"
                f.write(f"ParseAction(Action::{actEnum}, {action[1]})")
                if idx < len(row) - 1:
                    f.write(", ")

            f.write(" }")  # End Parse Row
            if rowIdx < len(table.table) - 1:
                f.write(",")
            f.write("\n")

        f.write("};\n")  # End Parse table

        f.write("}\n")  # End namespace Sigil


if __name__ == '__main__':
    main()
