from typing import Dict, Set, Optional
from rule import Rule
from errors import EBNFError
from log import logInfo, logErr, logWrn
from consts import END, INVALID, EMPTY

F_Set = Dict[str, Set[str]]


class Grammar:
    def __init__(self):
        self.terminals: Dict[str, str] = {END: END}
        self.rules: Set[Rule] = set()
        self._ruleNT: Set[str] = set()
        self.nulls: Set[str] = set()
        self.startSym: Optional[str] = None

        self.first: F_Set = {}
        self.follow: F_Set = {}

    def addNull(self, name: str):
        if name in self.nulls:
            logWrn(f'Name "{name}" has duplicate epsilon rule')
        else:
            self.nulls.add(name)

    def setStart(self, s: str):
        if self.startSym is None:
            self.startSym = s
        else:
            # TODO change to logDgb?
            logWrn(f'Start symbol already set to "{self.startSym}" ignoring attempt to set to "{s}"')

    def addRule(self, r: Rule) -> None:
        if r.nonterm in INVALID:
            raise EBNFError(f'Nonterminal "{r.nonterm}" is not a legal name')
        if r.nonterm in self.terminals:
            raise EBNFError(f'Name "{r.nonterm}" cannot be both terminal and nonterminal')

        if r in self.rules:
            raise EBNFError(f'Duplicate Rule detected: {r}')

        self.rules.add(r)
        self._ruleNT.add(r.nonterm)

    def addTerminal(self, name: str, val: str) -> None:
        if name in INVALID:
            raise EBNFError(f'Terminal "{name}" is not a legal name')
        if name in self.rules:
            raise EBNFError(f'Name "{name}" cannot be both terminal and nonterminal')
        if name in self.terminals:
            raise EBNFError(f'Terminal "{name}" already defined')

        self.terminals[name] = val

    def basicErrorCheck(self) -> bool:
        logInfo("Basic Error Checking")
        # Basic err/warn checking

        error = False
        alreadyErred = {EMPTY}
        if self.startSym is not None:
            used = {self.startSym}
        else:
            logErr("No Nonterminal rules added")
            return False

        for rule in self.rules:
            for nt in rule.symbols:
                if (nt not in self._ruleNT and nt not in self.terminals) and nt not in alreadyErred:
                    logErr(f'Error Check: No rule found for nonterminal "{nt}"')
                    error = True
                    alreadyErred.add(nt)

                used.add(nt)

        alreadyErred = {EMPTY, END}
        for rule in self.rules:
            if rule.nonterm not in used and rule.nonterm not in alreadyErred:
                logWrn(f'Error Check: Nonterminal "{rule.nonterm}" is not used in a rule')
                alreadyErred.add(rule.nonterm)

        for nt in self.terminals:
            if nt not in used and nt not in alreadyErred:
                logWrn(f'Error Check: Terminal "{nt}" is not used in a rule')
                alreadyErred.add(nt)

        return error

    def computeFirstAndFollow(self):
        if self.startSym is None:
            raise EBNFError(f'Start symbol not set, cannot compute First and Follow')

        first: Dict[str, Set[str]] = {x.nonterm: set() for x in self.rules}
        follow: Dict[str, Set[str]] = {x.nonterm: set() for x in self.rules}
        for x in self.terminals:
            first[x] = {x}
            follow[x] = set()

        for x in self.rules:
            if x.nonterm in self.nulls:
                first[x.nonterm].add(EMPTY)

        # Calculate "first" set
        logInfo('Computing "First" set')

        changed = True
        while changed:
            changed = False
            for rule in self.rules:
                leftFirst = first[rule.nonterm]
                priorEmpty = True
                for nt in rule.symbols:
                    if priorEmpty:
                        leftFirst = leftFirst.union(first[nt])
                        priorEmpty = nt in self.nulls

                if leftFirst > first[rule.nonterm]:
                    first[rule.nonterm] = leftFirst
                    changed = True

        # Calculate "follow" set
        logInfo('Computing "Follow" set')
        follow[self.startSym] = {END}

        changed = True
        while changed:
            changed = False
            for rule in self.rules:
                left = follow[rule.nonterm]
                lastEmpty = True
                for i in reversed(range(len(rule.symbols))):
                    val = rule.symbols[i]
                    if val == EMPTY:
                        break
                    if lastEmpty:
                        right = follow[val]
                        new = left.union(right)
                        if new > right:
                            follow[val] = new
                            changed = True
                        lastEmpty = EMPTY in first[val]
                    if i > 0:
                        prior = rule.symbols[i - 1]
                        curFirst = first[val].difference({EMPTY})
                        priorFollow = follow[prior]
                        new = priorFollow.union(curFirst)
                        if new > priorFollow:
                            follow[prior] = new
                            changed = True
                # END for rule
            # END for rules
        # END while changed

        for x in self.terminals:
            del follow[x]

        del first[END]

        self.first = first
        self.follow = follow
