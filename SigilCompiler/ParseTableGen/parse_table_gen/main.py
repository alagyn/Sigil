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


def validate_file(filename: str, g: Grammer, pt: ParseTable) -> bool:
    pass


def main():
    parser = ArgumentParser()
    parser.add_argument("grammer_file")

    args = parser.parse_args()
    grammer = parse_ebnf_file(args.grammer_file)
    ff = FirstAndFollow(grammer)
    lalr = LALR1Automata(grammer, ff)
    table = ParseTable(lalr)


if __name__ == '__main__':
    main()
