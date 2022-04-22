from typing import Dict, List, Set, Tuple, Optional
from rule import Rule
from errors import EBNFError
from log import logInfo, logErr, logWrn

EMPTY = 'EMPTY'
END = '$'

INVALID = {EMPTY, END}

F_Set = Dict[str, Set[str]]


class Grammer:
    def __init__(self):
        self.terminals: Dict[str, str] = {END: END}
        self.rules: Dict[str, List[Rule]] = {}
        self.nulls: Set[str] = set()
        self.goalNT: Optional[str] = None

    def addNull(self, name: str):
        if name in self.nulls:
            logWrn(f'Name "{name}" has duplicate epsilon rule')
        else:
            self.nulls.add(name)

    def setGoal(self, nonterm):
        if self.goalNT is None:
            self.goalNT = nonterm
        else:
            logWrn(f'Goal symbol already set to "{self.goalNT}" ignoring attempt to set to "{nonterm}"')

    def addRule(self, r: Rule) -> None:
        if r.nonterm in INVALID:
            raise EBNFError(f'Nonterminal "{r.nonterm}" is not a legal name')
        if r.nonterm in self.terminals:
            raise EBNFError(f'Name "{r.nonterm}" cannot be both terminal and nonterminal')

        if r.nonterm in self.rules:
            self.rules[r.nonterm].append(r)
        else:
            self.rules[r.nonterm] = [r]

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
        if self.goalNT is not None:
            used = {self.goalNT}
        else:
            used = set()
            error = True
            logErr(f'Goal state not set, add "@[goalStateHere];')

        for key, r in self.rules.items():
            for rule in r:
                for nt in rule.rule:
                    if (nt not in self.rules and nt not in self.terminals) and nt not in alreadyErred:
                        logErr(f'Error Check: No rule found for nonterminal "{nt}"')
                        error = True
                        alreadyErred.add(nt)

                    used.add(nt)

        alreadyErred = {EMPTY, END}
        for nt in self.rules:
            if nt not in used and nt not in alreadyErred:
                logWrn(f'Error Check: Nonterminal "{nt}" is not used in a rule')
                alreadyErred.add(nt)

        for nt in self.terminals:
            if nt not in used and nt not in alreadyErred:
                logWrn(f'Error Check: Terminal "{nt}" is not used in a rule')
                alreadyErred.add(nt)

        return error

    def computeFirstAndFollow(self) -> Tuple[F_Set, F_Set]:
        if self.goalNT is None:
            raise EBNFError(f'Goal state not set, cannot compute First and Follow')

        first: Dict[str, Set[str]] = {x: set() for x in self.rules.keys()}
        follow: Dict[str, Set[str]] = {x: set() for x in self.rules.keys()}
        for x in self.terminals:
            first[x] = {x}
            follow[x] = set()

        for x in self.rules:
            if x in self.nulls:
                first[x].add(EMPTY)


        # Calculate "first" set
        logInfo('Computing "First" set')

        changed = True
        while changed:
            changed = False
            for left, rs in self.rules.items():
                leftFirst = first[left]
                for rule in rs:
                    priorEmpty = True
                    for nt in rule.rule:
                        if priorEmpty:
                            leftFirst = leftFirst.union(first[nt])
                            priorEmpty = nt in self.nulls

                if leftFirst > first[left]:
                    first[left] = leftFirst
                    changed = True


        # Calculate "follow" set
        logInfo('Computing "Follow" set')
        follow[self.goalNT] = {END}

        changed = True
        while changed:
            changed = False
            for nt, ruleList in self.rules.items():
                left = follow[nt]
                for rule in ruleList:
                    lastEmpty = True
                    for i in reversed(range(len(rule.rule))):
                        val = rule.rule[i]
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
                            prior = rule.rule[i - 1]
                            curFirst = first[val].difference({EMPTY})
                            priorFollow = follow[prior]
                            new = priorFollow.union(curFirst)
                            if new > priorFollow:
                                follow[prior] = new
                                changed = True
                    # END for rule
                # END for rulelist
            # END for rules
        # END while changed

        for x in self.terminals:
            del follow[x]

        del first[END]

        return first, follow
