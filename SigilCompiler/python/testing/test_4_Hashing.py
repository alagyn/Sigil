import unittest

from lr1Grammer import Node, AnnotRule
from rule import Rule

class TestHashing(unittest.TestCase):
    def test_ruleHash(self):

        r1 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])
        r2 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])
        r3 = Rule('NONTERM', ['TERMINAL', 'ANOTHER'])
        r4 = Rule('asdf', ['TERMINAL', 'ANOTHER', 'whatIsThis'])

        self.assertEqual(hash(r1), hash(r2), 'Rule hashes are not equal')
        self.assertNotEqual(hash(r1), hash(r3), 'Rule hashes are equal')
        self.assertNotEqual(hash(r1), hash(r4), 'Rule hashes are equal')
        self.assertNotEqual(hash(r4), hash(r3), 'Rule hashes are equal')


    def test_annotRuleHash(self):
        r1 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])
        r2 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])

        ar1 = AnnotRule(r1, 0, {'$'})
        ar2 = AnnotRule(r2, 0, {'$'})
        self.assertEqual(hash(ar1), hash(ar2))

        ar3 = AnnotRule(r1, 0, {'$', 'T'})
        self.assertEqual(ar1.lookupHash(), ar3.lookupHash())
        self.assertNotEqual(hash(ar1), hash(ar3))

        ar4 = AnnotRule(r1, 1, {'$'})

        self.assertNotEqual(ar1.lookupHash(), ar4.lookupHash())
        self.assertNotEqual(hash(ar1), hash(ar4))


    def test_nodeHash(self):
        r1 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])
        r2 = Rule('NONTERM', ['TERMINAL', 'ANOTHER', 'whatIsThis'])

        n1 = Node()
        n1.addRule(r1, 0, {'$'})
        n1.freeze()

        n2 = Node()
        n2.addRule(r2, 0, {'$'})
        n2.freeze()

        self.assertEqual(hash(n1), hash(n2), 'Node hashes not equal')

        n3 = Node()
        n3.addRule(r2, 1, {'$'})
        n3.freeze()

        self.assertNotEqual(hash(n1), hash(n3))

        n4 = Node()
        n4.addRule(r2, 0, {'3'})
        n4.freeze()

        self.assertNotEqual(hash(n1), hash(n4))

        n5 = Node()
        n5.addRule(r1, 0, {'$'})
        n5.addRule(r1, 1, {'$'})
        n5.freeze()

        self.assertNotEqual(hash(n1), hash(n5))