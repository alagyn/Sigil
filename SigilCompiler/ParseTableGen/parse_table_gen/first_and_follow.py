from typing import Dict

from parse_table_gen.ebnf_grammer import Grammer
from parse_table_gen.consts import EMPTY, END

class FirstAndFollow:
    def __init__(self, grammer: Grammer) -> None:
        self.first: Dict[str, set[str]] = {}
        self.follow: Dict[str, set[str]] = {}

        self.first: Dict[str, set[str]] = {}
        for symbol in grammer.symbols:
            self.first[symbol] = set()
            if symbol in grammer.nulls:
                self.first[symbol].add(EMPTY)

        # Initialize the .irst set to contain the terminals
        for term in grammer.terminals:
            self.first[term] = set([term])

        changed = True
        while changed:
            changed = False
            for rule in grammer.rules:
                curSet = self.first[rule.nonterm].copy()

                nullable = True
                for symbol in rule.symbols:
                    curSet.update(self.first[symbol])
                    if symbol not in grammer.nulls:
                        nullable = False
                        break

                if nullable:
                    curSet.add(EMPTY)

                if curSet > self.first[rule.nonterm]:
                    self.first[rule.nonterm] = curSet
                    changed = True
            # End for rule
        # End while changed

        self.follow[grammer.startSymbol] = {END}

        changed = True
        while changed:
            changed = False
            for rule in grammer.rules:
                left = self.follow[rule.nonterm]
                # True if the previous symbol had EMPTy in their first set
                lastEmpty = True
                # Iterate backwards so to keep track of when the next symbol can be empty
                for i in reversed(range(len(rule.symbols))):
                    curSymbol = rule.symbols[i]
                    if curSymbol == EMPTY:
                        break

                    if lastEmpty:
                        right = self.follow[curSymbol]
                        new = left & right
                        if new > right:
                            self.follow[curSymbol] = new
                            changed = True
                        lastEmpty = EMPTY in self.first[curSymbol]
                    if i > 0:
                        prior = rule.symbols[i - 1]
                        curFirst = self.first[curSymbol] | {EMPTY}
                        priorFollow = self.follow[prior]
                        new = priorFollow.union(curFirst)
                        if new > priorFollow:
                            self.follow[prior] = new
                            changed = True
                # End for symbols
            # End for rules
        # End while changed

        for x in grammer.terminals:
            del self.follow[x]

        del self.first[END]



