from typing import List, Set, Dict, Tuple

from grammar import Grammar
from rule import Rule
from consts import END
from errors import EBNFError
from annotatedRule import AnnotRule
from collections import deque


class _NodeIter:
    def __init__(self, rules: List[AnnotRule]):
        self._rules = rules
        self._iterIdx = -1

    def __next__(self) -> AnnotRule:
        self._iterIdx += 1
        if self._iterIdx >= len(self._rules):
            raise StopIteration
        return self._rules[self._iterIdx]


class Node:
    _STATE_ID_GEN = 0

    @classmethod
    def decID(cls):
        cls._STATE_ID_GEN -= 1

    def __init__(self):
        self._rulesD: Dict[AnnotRule, AnnotRule] = {}
        self._rules: List[AnnotRule] = []

        self._iterIdx = -1

        self.trans: Dict[str, Node] = {}

        self.stateID = Node._STATE_ID_GEN
        Node._STATE_ID_GEN += 1
        self._freeze = False
        self._hash = None

    def __iter__(self) -> _NodeIter:
        return _NodeIter(self._rules)

    def addTrans(self, nt: str, node: 'Node'):
        if nt in self.trans:
            raise EBNFError(f"Node: {str(self)}\n"
                            f"Attempted to add duplicate transition on NT: {nt}\n"
                            f"Existing: {str(self.trans[nt])}\n"
                            f"New: {str(node)}")

        self.trans[nt] = node

    def freeze(self):
        self._freeze = True
        self._hash = sum(hash(x) for x in self._rules)

    def addRule(self, r: Rule, idx: int, la: Set[str]) -> bool:
        """
        Attempts to add a rule to the node. If a duplicate is found,
        the new LA is merged into the existing rule.
        :param r: The rule to add
        :param idx: The idx of the next symbol
        :param la: The new LA
        :return: True if a change occurs
        """
        new = AnnotRule(r, idx, la)
        try:
            old = self._rulesD[new]
            return old.combine(new)
        except KeyError:
            pass

        if self._freeze:
            raise EBNFError(f"Node: {str(self)}\n"
                            f"Cannot add new rule: {str(new)}\n"
                            f"Node is frozen")
        self._rulesD[new] = new
        self._rules.append(new)
        return True

    def combine(self, other: 'Node') -> bool:
        out = False
        for ar1, ar2 in zip(self._rules, other._rules):
            if ar1 != ar2:
                raise EBNFError("Attempting to combine two unequal nodes")
            if ar1.combine(ar2):
                out = True
        return out

    def __hash__(self):
        if self._hash is None:
            raise EBNFError("Cannot hash node, it hasn't been frozen")

        return self._hash

    def __eq__(self, other):
        if not isinstance(other, Node):
            return False

        for r1, r2 in zip(self._rules, other._rules):
            if r1 != r2:
                return False

        return True

    def __str__(self):
        return f'State#{self.stateID}'


class LALRGrammar:
    def __init__(self, g: Grammar):
        self.g = g
        self.start = Node()

        self.ruleLookup: dict[str, List[Rule]] = {}
        for rule in self.g.rules:
            try:
                self.ruleLookup[rule.nonterm].append(rule)
            except KeyError:
                self.ruleLookup[rule.nonterm] = [rule]

        self.stateLookup: dict[Node, Node] = {}
        self.idLookup: dict[int, Node] = {}

    def makeClosure(self, node: Node):
        """
        Computes the LR(1) Closure of a node and updates it
        :param node: The node to update
        :return: None
        """
        changed = True
        while changed:
            changed = False
            for annotR in node:
                if annotR.indexAtEnd():
                    # If IDX is at the end, skip
                    continue

                nextSym = annotR.getNextSym()
                if nextSym in self.g.terminals:
                    # Skip if next symbol is terminal
                    continue

                newLA = annotR.getNewLA(self.g)

                for rule in self.ruleLookup[nextSym]:
                    x = node.addRule(rule, 0, newLA.copy())
                    if x:
                        changed = True
                # END for rule
            # END for annot rule
        # END while changed
        node.freeze()

    def makeNew(self, cur: Node, symbol: str) -> Node:
        new = Node()

        for annotR in cur:
            if not annotR.indexAtEnd() and annotR.getNextSym() == symbol:
                new.addRule(annotR.rule, annotR.idx + 1, annotR.copyLA())

        self.makeClosure(new)
        return new

    def resolveDupes(self, node: Node) -> Tuple[Node, bool]:
        try:
            old = self.stateLookup[node]
            # print(f'Duplicate detected {node.stateID} -> {old.stateID}')
            out = old.combine(node)
            # Dec Id since this is a duplicate node
            Node.decID()
            return old, out
        except KeyError:
            pass

        self.stateLookup[node] = node
        self.idLookup[node.stateID] = node
        return node, True

    def compute(self):
        # Make kernel for state 0
        for rule in self.ruleLookup[self.g.startSym]:
            self.start.addRule(rule, 0, {END})

        # print('Closing State#0')
        self.makeClosure(self.start)
        self.start.freeze()
        self.stateLookup[self.start] = self.start
        self.idLookup[0] = self.start

        # print('Building Graph')
        todo = deque([self.start])
        todos = {self.start}

        while len(todo) > 0:
            cur = todo.popleft()
            todos.remove(cur)
            used = set()
            for rule in cur:
                if not rule.indexAtEnd():
                    symbol = rule.getNextSym()
                    if symbol not in used:
                        used.add(symbol)
                        new = self.makeNew(cur, symbol)
                        new, changed = self.resolveDupes(new)
                        if symbol not in cur.trans:
                            print(f'{str(cur)}: Making trans on {symbol} -> {str(new)}')
                            cur.addTrans(symbol, new)
                        if changed and new not in todos:
                            todo.append(new)
                            todos.add(new)

        for x in range(len(self.idLookup)):
            node = self.idLookup[x]
            print(f'Node: {str(node)}')
            for rule in node:
                print(f'\t{str(rule)}')
        # print("Done")
