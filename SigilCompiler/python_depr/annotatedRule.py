from typing import Set

from grammar import Grammar
from rule import Rule
from errors import EBNFError


class AnnotRule:
    def __init__(self, rule: Rule, idx: int, la: Set[str]):
        self.rule = rule
        # IDX is idx of symbol AFTER dot, i.e. the idx == the symbol we are checking for
        self.idx = idx
        self._la = la

    def __eq__(self, other):
        if not isinstance(other, AnnotRule):
            return False

        return self.rule == other.rule and self.idx == other.idx

    def combine(self, other: 'AnnotRule') -> bool:
        """
        Adds the LA of the other rule. Returns True if it added new items
        :param other: The rule to combine
        :return: True if changes occur
        """
        new = self._la.union(other._la)
        if new > self._la:
            # Return true if self LA is proper superset of other LA
            self._la = new
            return True
        return False

    def indexBeforeEnd(self) -> bool:
        # dot is before the last symbol, i.e. IDX == len - 1
        return self.idx == len(self.rule.symbols) - 1

    def indexAtEnd(self) -> bool:
        return self.idx == len(self.rule.symbols)

    def getNextSym(self) -> str:
        if self.indexAtEnd():
            raise EBNFError("Cannot get next symbol, dot at end of input")

        return self.rule.symbols[self.idx]

    def copyLA(self):
        return set(self._la.copy())

    def refLA(self):
        return self._la

    def getNewLA(self, g: Grammar) -> Set[str]:
        """
        Returns the lookahead for closure rules generated from this rule
        Uses set of symbols that can be collapsed after the next symbol to be
        consumed (i.e. every symbol from idx + 1 up to and including
        the first that can't be null)
        :param g: The grammar
        :return: The lookahead
        """
        out = set()
        for i in range(self.idx + 1, len(self.rule.symbols)):
            out.update(g.first[self.rule.symbols[i]])
            if self.rule.symbols[i] not in g.nulls:
                return out

        # If we got here, every symbol after the next can be nulled
        out.update(self._la)
        return out

    def __hash__(self):
        return hash(self.rule) + hash(self.idx)

    def __str__(self):
        rl = ''
        for i in range(len(self.rule.symbols)):
            if i == self.idx:
                rl += ' .'
            rl += f' {self.rule.symbols[i]}'

        if self.idx == len(self.rule.symbols):
            rl += ' .'

        la = ', '.join(self._la)
        return f'{self.rule.nonterm} ={rl} {{{la}}}'