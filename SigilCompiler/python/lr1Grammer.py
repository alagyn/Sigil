from typing import List, Set, Dict

from grammer import Grammer
from rule import Rule
from consts import END
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

        return hash(self.rule) == hash(other.rule) and self.idx == other.idx

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

    def getNewLA(self, g: Grammer) -> Set[str]:
        """
        Returns the lookahead for closure rules generated from this rule
        Uses set of symbols that can be collapsed after the next symbol to be
        consumed (i.e. every symbol from idx + 1 up to and including
        the first that can't be null)
        :param g: The grammer
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

    def lookupHash(self):
        return hash(self.rule) + hash(self.idx)

    def __hash__(self):
        return hash(self.rule) + hash(self.idx) + hash(frozenset(self._la))

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
        self._rulesD: Dict[int, AnnotRule] = {}
        self._rules: List[AnnotRule] = []

        self._iterIdx = -1

        self.trans: Dict[str, Node] = {}

        self.stateID = Node._STATE_ID_GEN
        Node._STATE_ID_GEN += 1

        self._hash = None
        self._frozenRules = None

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
        if self._frozenRules is not None:
            raise EBNFError(f"Node: {str(self)}\n"
                            f"Cannot add rule, node is frozen\n"
                            f"Rule: {str(new)}")
        try:
            old = self._rulesD[new.lookupHash()]
            return old.combine(new)
        except KeyError:
            pass

        self._rulesD[new.lookupHash()] = new
        self._rules.append(new)
        return True

    def freeze(self):
        self._frozenRules = frozenset(self._rules)
        self._hash = hash(self._frozenRules)
        del self._rulesD

    def __hash__(self):
        if self._hash is None:
            raise EBNFError("Cannot hash node, it hasn't been frozen")

        return self._hash

    def __str__(self):
        return f'State#{self.stateID}'


class LROneGrammer:
    def __init__(self, g: Grammer):
        self.g = g
        self.start = Node()

        self.ruleLookup: dict[str, List[Rule]] = {}
        for rule in self.g.rules:
            try:
                self.ruleLookup[rule.nonterm].append(rule)
            except KeyError:
                self.ruleLookup[rule.nonterm] = [rule]

        self.stateLookup: dict[int, Node] = {}

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

    def checkState(self, node: Node) -> Node:
        try:
            old = self.stateLookup[hash(node)]
            # print(f'Duplicate detected {node.stateID} -> {old.stateID}')
            # Dec Id since this is a duplicate node
            Node.decID()
            return old
        except KeyError:
            pass

        self.stateLookup[hash(node)] = node
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
        self.stateLookup[hash(self.start)] = self.start

        # print('Building Graph')
        self.recurBuildGraph(self.start)
        # print("Done")
