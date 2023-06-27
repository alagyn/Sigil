from collections import deque
from typing import Deque, Set

from pyvis.network import Network
from networkx import DiGraph
import matplotlib.pyplot as plt

def generateNetwork(grammar):
    # net = Network(directed=True, height='100%', width='100%')
    G = DiGraph()

    G.add_nodes_from([x for x in range(len(grammar.stateLookup))])

    for node in grammar.stateLookup.values():
        # net.add_node(node.stateID, f"{node.stateID}\n" + "\n".join(str(rule) for rule in node))
        for key, val in node.trans.items():
            G.add_edge(node.stateID, val.stateID)

    net = Network("100%", "100%", directed=True)
    net.from_nx(G)
    net.barnes_hut(spring_length=10, overlap=0.5)
    net.show("Sigil.html")