from typing import List, Dict, Set, Tuple
import re


class Rule:

    def __init__(self, nonterm: str, symbols: List[str]) -> None:
        self.nonterm = nonterm
        self.symbols = symbols

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Rule):
            return False

        return self.nonterm == other.nonterm and self.symbols == other.symbols

    def __str__(self) -> str:
        return f'Rule: {self.nonterm} = {" ".join(self.symbols)}'

    def compare(self, other) -> int:
        if not isinstance(other, Rule):
            raise NotImplementedError(f'Cannot compare Rule to {type(other)}')

        snt = self.nonterm.lower()
        ont = other.nonterm.lower()

        if snt < ont:
            return -1
        elif snt > ont:
            return 1

        if len(self.symbols) < len(other.symbols):
            return -1
        if len(self.symbols) > len(other.symbols):
            return 1

        for x, y in zip(self.symbols, other.symbols):
            x = x.lower()
            y = y.lower()
            if x < y:
                return -1
            if x > y:
                return 1

        return 0

    def __lt__(self, other):
        return self.compare(other) == -1

    def __le__(self, other):
        return self.compare(other) <= 0

    def __gt__(self, other):
        return self.compare(other) == 1

    def __ge__(self, other):
        return self.compare(other) >= 0


class Grammer:

    def __init__(self, terminals: List[Tuple[str, str]], rules: List[Rule], nulls: Set[str]) -> None:
        # List of each rule
        self.rules = rules
        # Set of nonterminals that can go to null
        self.nulls = nulls
        # Map of name -> regex
        self.terminalList = terminals
        self.terminals = {
            x[0]: x[1]
            for x in self.terminalList
        }

        # The set of every symbol defined in our grammer
        self.symbols = set(self.terminals.keys())
        for rule in self.rules:
            self.symbols.add(rule.nonterm)
            self.symbols.update(rule.symbols)
        # Set the start symbol to the first rule
        self.startSymbol = self.rules[0].nonterm


TERMINAL_RE = re.compile(r'(?P<name>\w+)\s*=\s*(?P<regex>(\'[^\']+\')|("[^"]+"));')
EMPTY_RE = re.compile(r'(?P<nonterm>\w+)\s*=\s*EMPTY\s*;')
RULE_RE = re.compile(r'(?P<nonterm>\w+)\s*=\s*(?P<symbols>(\w+(\s+\w+)*)(\s*\|\s*\w+(\s+\w+)*)*);')


def parse_grammer(lines: List[str]) -> Grammer:
    terminalNames = set()
    terminals: List[Tuple[str, str]] = []
    rules: List[Rule] = []
    nonterminals = set()
    nulls = set()

    error = False
    for line in lines:
        m = TERMINAL_RE.fullmatch(line)
        if m is not None:
            name = m.group("name")
            regex = m.group("regex")
            if regex.startswith('"'):
                regex = regex.strip('"')
            else:
                regex = regex.strip("'")
            if name in terminalNames:
                raise RuntimeError(f'Duplicate terminal definition: "{name}"')

            terminals.append((name, regex))
            terminalNames.add(name)
            continue

        m = EMPTY_RE.fullmatch(line)
        if m is not None:
            nonterm = m.group('nonterm')
            nulls.add(nonterm)
            continue

        m = RULE_RE.fullmatch(line)
        if m is not None:
            nonterm = m.group("nonterm")
            rule_list = m.group("symbols").split('|')
            for symbol_str in rule_list:
                symbol_str = symbol_str.strip()
                symbols = []
                for x in symbol_str.split(" "):
                    x = x.strip()
                    if len(x) > 0:
                        symbols.append(x)
                nonterminals.add(nonterm)
                rules.append(Rule(nonterm, symbols))
            continue

        print("Invalid line:", line)
        error = True

    if error:
        raise RuntimeError("Bad Parse")

    for rule in rules:
        for symbol in rule.symbols:
            if symbol not in terminalNames and symbol not in nonterminals:
                raise RuntimeError(f"Missing terminal/nonterminal definitions for symbol: {symbol}")

        if rule.nonterm in terminals:
            raise RuntimeError(f'Symbol defined as both a terminal and nonterminal: "{rule.nonterm}"')

    return Grammer(terminals, rules, nulls)
