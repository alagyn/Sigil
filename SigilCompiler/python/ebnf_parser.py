import re
from typing import List, Dict, Set, Tuple

from rule import Rule
from grammar import Grammar
from lalrGrammar import LALRGrammar
from errors import EBNFError
from log import logErr, logInfo, logWrn
import visualizeLR1

NONTERM = 'nonterm'
DEFIN = 'defin'
TERM = 'term'

RULE_RE = re.compile(r'(?P<nonterm>\w+) *= *(?P<defin>(\w+ *)+);')
TERM_RE = re.compile(r'(?P<nonterm>\w+) *= *(?P<term>("[^"]+")|(\'[^\']+\')) *;')
EPSI_RE = re.compile(r'(?P<nonterm>\w+) *= *EMPTY *;')

COMMENT_RE = re.compile(r'((\'.*\')|(".*")|[^\'"])* *;? *(?P<comment>#.*)')


def _plist(l):
    print("START")
    for x in l:
        print(x)
    print("END")


def cleanInput(filename) -> List[str]:
    """
    Reads input file and removes comments and strips whitespace
    :param filename: The file to read
    :return: The cleaned lines
    """
    # Read file
    with open(filename, mode='r') as f:
        lines = f.readlines()

    cleanLines = []
    logInfo("Cleaning EBNF File Input")
    # Remove comments/empty lines
    for idx in range(len(lines)):
        line = lines[idx].strip()
        if len(line) > 0:
            match = COMMENT_RE.fullmatch(line)
            # Strip comments
            if match is not None:
                start, _ = match.span("comment")
                line = line[0: start].strip()

            if len(line) > 0:
                cleanLines.append(line)

    return cleanLines


def combineLines(lines: List[str]) -> List[str]:
    """
    Combines the multiline rules into single lines
    :param lines: The cleaned lines from input file
    :return: A list of each rule as a single string
    """
    # Combine multi-line rules into single lines
    ruleLines = []
    curLine = ''
    for line in lines:
        # Concat next line
        curLine += ' ' + line
        # Check for ending semicolon
        if curLine[-1] == ';':
            # If so, add rule
            ruleLines.append(curLine.strip())
            # Reset current
            curLine = ''

    if len(curLine) > 0 and curLine[-1] != ';':
        raise EBNFError("Missing final semicolon")

    return ruleLines

def parseGrammar(ruleLines: List[str]) -> Grammar:
    # List of rules
    grammar = Grammar()

    # Parse rules
    logInfo("Parsing EBNF Rules")
    errored = False
    startFound = False
    for line in ruleLines:
        # Epsilon
        match = EPSI_RE.fullmatch(line)
        if match is not None:
            if not startFound:
                logWrn("Epsilon rule declared before start rule")
            nt = match.group(NONTERM)
            grammar.addNull(nt)
            continue

        # Terminal
        match = TERM_RE.fullmatch(line)
        if match is not None:
            nt = match.group(NONTERM)
            term = match.group(TERM)
            # Strip qutoes
            term = term[1:-1]
            try:
                grammar.addTerminal(nt, term)
            except EBNFError as e:
                logErr(str(e))
                errored = True
            continue

        # Rule
        match = RULE_RE.fullmatch(line)
        if match is not None:
            nt = match.group(NONTERM)
            defin = match.group(DEFIN)

            r = Rule(nt, defin.split(' '))
            try:
                grammar.addRule(r)
            except EBNFError as e:
                logErr(str(e))
                errored = True

            if not startFound:
                grammar.setStart(nt)
                startFound = True

            continue

        logErr(f"Invalid rule, skipping: {line}")
        errored = True

    if errored or grammar.basicErrorCheck():
        raise EBNFError("Parse errors occured, exitting")

    return grammar

def parseEBNFFile(filename) -> Grammar:
    lines = cleanInput(filename)
    ruleLines = combineLines(lines)
    return parseGrammar(ruleLines)

def parse(filename):

    grammar = parseEBNFFile(filename)
    grammar.computeFirstAndFollow()

    lalrG = LALRGrammar(grammar)
    lalrG.compute()

    visualizeLR1.generateNetwork(lalrG)


if __name__ == '__main__':
    import sys


    def _main():
        parse(sys.argv[1])


    _main()
