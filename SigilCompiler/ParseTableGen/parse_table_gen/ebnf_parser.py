import re
from typing import List
from argparse import ArgumentParser

import parse_table_gen.preprocessing as preprocess
import parse_table_gen.ebnf_grammer as ebnf_grammer

def parse_ebnf_file(filename: str) -> ebnf_grammer.Grammer:
    cleanLines = preprocess.read_and_clean(filename)
    combinedLines = preprocess.combine_lines(cleanLines)
    del cleanLines

    return ebnf_grammer.parse_grammer(combinedLines)

def main():
    parser = ArgumentParser()
    parser.add_argument("grammer_file")

    args = parser.parse_args()
    parse_ebnf_file(args.grammer_file)

if __name__ == '__main__':
    main()



