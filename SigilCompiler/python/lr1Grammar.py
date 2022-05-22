from typing import List, Set, Dict

from grammar import Grammar
from rule import Rule
from consts import END
from errors import EBNFError
from annotatedRule import AnnotRule

class _NodeIter:
    def __init__(self, rules: List[AnnotRule]):
        self._rules = rules
        self._iterIdx = -1

    def __next__(self):
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

        self._hash = None

    def __iter__(self):
        return _NodeIter(self._rules)

    def addTrans(self, nt: str, node: 'Node'):
        if nt in self.trans:
            raise EBNFError(f"Node: {str(self)}\n"
                            f"Attempted to add duplicate transition on NT: {nt}\n"
                            f"Existing: {str(self.trans[nt])}\n"
                            f"New: {str(node)}")

        self.trans[nt] = node

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

        self._rulesD[new] = new
        self._rules.append(new)
        return True

    def __eq__(self, other):
        if not isinstance(other, Node):
            return False

        for r1, r2 in zip(self._rules, other._rules):
            if r1 != r2:
                return False

        return True

    def __hash__(self):
        if self._hash is None:
            raise EBNFError("Cannot hash node, it hasn't been frozen")

        return self._hash

    def __str__(self):
        return f'State#{self.stateID}'


class LROneGrammar:
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

    def checkState(self, node: Node) -> Node:
        try:
            old = self.stateLookup[node]
            # print(f'Duplicate detected {node.stateID} -> {old.stateID}')
            # Dec Id since this is a duplicate node
            Node.decID()
            return old
        except KeyError:
            pass

        self.stateLookup[node] = node
        # Recurse
        self.recurBuildGraph(node)
        return node

    def requestNode(self, parent: Node, trans: str) -> Node:
        new = Node()

        for annotR in parent:
            if not annotR.indexAtEnd() and annotR.getNextSym() == trans:
                new.addRule(annotR.rule, annotR.idx + 1, annotR.copyLA())

        self.makeClosure(new)

        return new

    def recurBuildGraph(self, node: Node):
        # print("Recur Build", node.stateID)
        # for rule in node:
            # print(f'\t{str(rule)}')

        used = set()
        for rule in node:
            if not rule.indexAtEnd():
                symbol = rule.getNextSym()
                if symbol not in used:
                    used.add(symbol)
                    new = self.requestNode(node, symbol)
                    print(f'{str(node)}: Making trans on {symbol} -> {str(new)}')
                    new = self.checkState(new)
                    node.addTrans(symbol, new)

    def compute(self):
        # Make kernel for state 0
        for rule in self.ruleLookup[self.g.startSym]:
            self.start.addRule(rule, 0, {END})

        # print('Closing State#0')
        self.makeClosure(self.start)
        self.stateLookup[self.start] = self.start

        # print('Building Graph')
        self.recurBuildGraph(self.start)
        # print("Done")
