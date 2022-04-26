from collections import deque
from typing import Deque, Set

from pyvis.network import Network
import matplotlib.pyplot as plt

from lr1Grammar import LROneGrammar, Node

def generateNetwork(grammar: LROneGrammar):
    net = Network(directed=True, height='100%', width='100%')

    for node in grammar.stateLookup.values():
        net.add_node(node.stateID, f"{node.stateID}\n" + "\n".join(str(rule) for rule in node))

    visited: Set[int] = set()
    todo: Deque[Node] = deque()
    todo.append(grammar.start)

    labels = {}

    while len(todo) > 0:
        cur = todo.pop()
        visited.add(cur.stateID)
        for t, n in cur.trans.items():
            net.add_edge(cur.stateID, n.stateID, title=t)
            labels[(cur.stateID, n.stateID)] = t
            if n.stateID not in visited:
                todo.append(n)

    net.barnes_hut(spring_length=10, overlap=0.5)
    net.show("Sigil.html")