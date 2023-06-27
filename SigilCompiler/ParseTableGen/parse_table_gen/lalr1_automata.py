from typing import Dict, List, Tuple
from collections import deque

from parse_table_gen.ebnf_grammer import Grammer, Rule
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.consts import END


class AnnotRule:
    """
    A rule combined with a parse index and a look ahead
    """

    def __init__(self, rule: Rule, parseIndex: int, lookAhead: set[str]) -> None:
        """
        :param rule: The Rule
        :param parseIndex: The index of the symbol AFTER the dot, i.e. the symbol we are looking for
        :param lookAhead: The look ahead set for the rule
        """
        self.rule = rule
        self.parseIndex = parseIndex
        self.lookAhead = lookAhead

    def __str__(self) -> str:
        out = f'AnnotRule: {self.rule.nonterm} ='

        for i in range(self.parseIndex):
            out += " " + self.rule.symbols[i]

        out += " *"

        for i in range(self.parseIndex, len(self.rule.symbols)):
            out += " " + self.rule.symbols[i]

        return out

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, AnnotRule):
            return False

        return self.rule == other.rule and self.parseIndex == other.parseIndex

    def combine(self, other: 'AnnotRule') -> bool:
        """
        Adds the look ahead of the other rule.
        :param other: The rule to combine
        :return: True if look ahead was changed
        """
        new = self.lookAhead.union(other.lookAhead)
        if new > self.lookAhead:
            self.lookAhead = new
            return True
        return False

    def indexAtEnd(self) -> bool:
        """
        Check if the dot is just before the last symbol
        """
        return self.parseIndex == len(self.rule.symbols) - 1

    def nextSymbol(self) -> str:
        """
        Get the next symbol
        """
        return self.rule.symbols[self.parseIndex]

    def getNewLA(self, g: Grammer, ff: FirstAndFollow) -> set[str]:
        """
        Returns the lookahead for closure rules generated from this rule
        Uses set of symbols that can be collapsed after the next symbol to be
        consumed (i.e. every symbol from idx + 1 up to and including
        the first that can't be null)
        :param g: The grammer
        :param ff: The first and follow sets
        :return: The lookahead
        """

        out = set()
        for i in range(self.parseIndex + 1, len(self.rule.symbols)):
            symbol = self.rule.symbols[i]
            out.update(ff.first[symbol])
            if symbol not in g.nulls:
                return out

        # If we got here, every symbol after the next can be nulled
        # Add our own look ahead
        out.update(self.lookAhead)
        return out


class Node:
    NODE_ID_GEN = 0

    def __init__(self) -> None:
        self.id = Node.NODE_ID_GEN
        Node.NODE_ID_GEN += 1
        self.rules: List[AnnotRule] = []
        # Map of transitions
        self.trans: Dict[str, Node] = {}

    def __str__(self) -> str:
        return f'Node#{self.id}, num rules: {len(self.rules)}'

    def addRule(self, rule: Rule, parseIndex: int, lookAhead: set[str]) -> bool:
        """
        Attempts to add a rule to the node. If a duplicate is found,
        the new LA is merged into the existing rule.
        :param rule: The rule to add
        :param parseIndex: The idx of the next symbol
        :param lookAhead: The new LA
        :return: True if a change occurs
        """
        newRule = AnnotRule(rule, parseIndex, lookAhead)
        for annotRule in self.rules:
            if annotRule == newRule:
                return annotRule.combine(newRule)
        return False

    def __eq__(self, other: object) -> bool:
        if not isinstance(other, Node):
            return False

        if len(self.rules) != len(other.rules):
            return False

        for r1, r2 in zip(self.rules, other.rules):
            if r1 != r2:
                return False

        return True

    def combine(self, other: 'Node') -> bool:
        out = False
        for ar1, ar2 in zip(self.rules, other.rules):
            if ar1 != ar2:
                raise RuntimeError("Attempting to combine unequal nodes")
            if ar1.combine(ar2):
                out = True

        return out

    def addTrans(self, nt: str, node: 'Node'):
        if nt in self.trans:
            raise RuntimeError(
                f"Node: {str(self)}\n"
                f"Attempted to add duplicate transition on NT: {nt}\n"
                f"Existing: {str(self.trans[nt])}\n"
                f"New: {str(node)}"
            )

        self.trans[nt] = node


class LALR1Automata:

    def __init__(self, g: Grammer, ff: FirstAndFollow) -> None:
        self.start = Node()

        self.grammer = g
        self.ff = ff

        self.ruleLookup: Dict[str, List[Rule]] = {}
        for rule in g.rules:
            try:
                self.ruleLookup[rule.nonterm].append(rule)
            except KeyError:
                self.ruleLookup[rule.nonterm] = [rule]

        # Add all the rules for the start symbol to the start node
        for rule in self.ruleLookup[g.startSymbol]:
            self.start.addRule(rule, 0, {END})

        # Make the closure for the start node
        self.makeClosure(self.start)

        self.nodes: List[Node] = [self.start]

        # Make the todo queue
        todo = deque([self.start])

        while len(todo) > 0:
            cur = todo.popleft()
            used = set()
            for rule in cur.rules:
                if not rule.indexAtEnd():
                    symbol = rule.nextSymbol()
                    if symbol not in used:
                        used.add(symbol)
                        newNode = self.makeNewNode(cur, symbol)
                        new, changed = self.resolveDupes(newNode)
                        if symbol not in cur.trans:
                            cur.addTrans(symbol, new)
                        if changed and new not in todo:
                            todo.append(new)

    def makeClosure(self, node: Node) -> None:
        """
        Compute the LR(1) Closure of a node
        :param node: The node to compute
        :return: None
        """
        changed = True
        while changed:
            changed = False
            for annotRule in node.rules:
                if annotRule.indexAtEnd():
                    # The index is at the end, skip
                    continue

                nextSym = annotRule.nextSymbol()
                if nextSym in self.grammer.terminals:
                    # the next symbol is a terminal, skip
                    continue

                newLookAhead = annotRule.getNewLA(self.grammer, self.ff)

                for rule in self.ruleLookup[nextSym]:
                    if node.addRule(rule, 0, newLookAhead.copy()):
                        changed = True
                # End for rule
            # End for annotRule
        # End while changed

    def makeNewNode(self, curNode: Node, symbol: str) -> Node:
        newNode = Node()

        for annotR in curNode.rules:
            if not annotR.indexAtEnd() and annotR.nextSymbol() == symbol:
                newNode.addRule(annotR.rule, annotR.parseIndex + 1, annotR.lookAhead.copy())

        self.makeClosure(newNode)
        return newNode

    def resolveDupes(self, newNode: Node) -> Tuple[Node, bool]:
        for node in self.nodes:
            if node == newNode:
                return node, node.combine(newNode)

        # If we got here it is a new node
        self.nodes.append(newNode)
        return newNode, True