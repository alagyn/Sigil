import unittest

import utils

from parse_table_gen.lalr1_automata import Node, AnnotRule, LALR1Automata
from parse_table_gen.main import parse_ebnf_file
from parse_table_gen.ebnf_grammer import Grammer, Rule
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.consts import END


class TestLALRClosure(unittest.TestCase):

    def test_Closure(self):
        testfile = utils.getTestFilename('LALR1Test.sebnf')
        grammer = parse_ebnf_file(testfile)
        ff = FirstAndFollow(grammer)

        lalr = LALR1Automata(grammer, ff)

        SP = 'S_PRIME'
        S = 'S'
        X = 'X'
        a = 'a'
        b = 'b'

        r0 = Rule(SP, [S])
        r1 = Rule(S, [X, X])
        r2 = Rule(X, [a, X])
        r3 = Rule(X, [b])

        n0 = Node()
        n0.addRule(r0, 0, set([END]))
        n0.addRule(r1, 0, set([END]))
        n0.addRule(r2, 0, set([a, b]))
        n0.addRule(r3, 0, set([a, b]))

        n1 = Node()
        n1.addRule(r0, 1, set([END]))

        n2 = Node()
        n2.addRule(r1, 1, set([END]))
        n2.addRule(r2, 0, set([END]))
        n2.addRule(r3, 0, set([END]))

        n36 = Node()
        n36.addRule(r2, 1, set([a, b, END]))
        n36.addRule(r2, 0, set([a, b, END]))
        n36.addRule(r3, 0, set([a, b, END]))

        n5 = Node()
        n5.addRule(r1, 2, set([END]))

        n47 = Node()
        n47.addRule(r3, 1, set([a, b, END]))

        n89 = Node()
        n89.addRule(r2, 2, set([a, b, END]))

        EXP_NODES = [n0, n1, n2, n36, n47, n5, n89]

        for exp, act in zip(EXP_NODES, lalr.nodes):
            for expRule, actRule in zip(exp.rules, act.rules):
                self.assertEqual(expRule, actRule, f"Rule not equal, Exp: {expRule}, Act: {actRule}")

        self.assertEqual(len(EXP_NODES), len(lalr.nodes))
