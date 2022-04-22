import os.path
import unittest

import ebnf_parser
from testConsts import TEST_ROOT

class TestFirstAndFollow(unittest.TestCase):
    def test_FandF1(self):
        """
        From compiler book by Thain
        """
        testFile = os.path.join(TEST_ROOT, 'FandFtest1.ebnf')

        g = ebnf_parser.parseEBNFFile(testFile)

        first, follow = g.computeFirstAndFollow()

        EXP_FIRST = {
            'P': {'open_p', 'int'},
            'E': {'open_p', 'int'},
            'EP': {'plus', 'EMPTY'},
            'T': {'open_p', 'int'},
            'TP': {'star', 'EMPTY'},
            'F': {'open_p', 'int'},
            'plus': {'plus'},
            'star': {'star'},
            'open_p': {'open_p'},
            'close_p': {'close_p'},
            'int': {'int'}
        }

        self.assertEqual(len(EXP_FIRST), len(EXP_FIRST), 'Len of first def not equal')

        for key, val in first.items():
            self.assertTrue(key in EXP_FIRST, 'Name not in epected First dict')
            expSet = EXP_FIRST[key]
            self.assertEqual(len(expSet), len(val), 'Size of First set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected First set')

        EXP_FOLLOW = {
            'P': {'$'},
            'E': {'close_p', '$'},
            'EP': {'close_p', '$'},
            'T': {'close_p', 'plus', '$'},
            'TP': {'plus', 'close_p', '$'},
            'F': {'plus', 'star', 'close_p', '$'}
        }

        for key, val in follow.items():
            self.assertTrue(key in EXP_FOLLOW, 'Name not in epected Follow dict')
            expSet = EXP_FOLLOW[key]
            self.assertEqual(len(expSet), len(val), 'Size of Follow set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected Follow set')

    def testFandF2(self):
        """
        Test from https://people.cs.pitt.edu/~jmisurda/teaching/cs1622/handouts/cs1622-first_and_follow.pdf
        """
        testFile = os.path.join(TEST_ROOT, 'FandFtest2.ebnf')

        g = ebnf_parser.parseEBNFFile(testFile)

        first, follow = g.computeFirstAndFollow()

        EXP_FIRST = {
            'Y': {'star', 'EMPTY'},
            'X': {'plus', 'EMPTY'},
            'T': {'open_p', 'int'},
            'E': {'open_p', 'int'},
            'plus': {'plus'},
            'star': {'star'},
            'open_p': {'open_p'},
            'close_p': {'close_p'},
            'int': {'int'}
        }

        self.assertEqual(len(EXP_FIRST), len(EXP_FIRST), 'Len of first def not equal')

        for key, val in first.items():
            self.assertTrue(key in EXP_FIRST, 'Name not in epected First dict')
            expSet = EXP_FIRST[key]
            self.assertEqual(len(expSet), len(val), 'Size of First set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected First set')

        EXP_FOLLOW = {
            'Y': {'close_p', '$', 'plus'},
            'X': {'close_p', '$'},
            'T': {'close_p', '$', 'plus'},
            'E': {'close_p', '$'}
        }

        for key, val in follow.items():
            self.assertTrue(key in EXP_FOLLOW, 'Name not in epected Follow dict')
            expSet = EXP_FOLLOW[key]
            self.assertEqual(len(expSet), len(val), 'Size of Follow set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected Follow set')