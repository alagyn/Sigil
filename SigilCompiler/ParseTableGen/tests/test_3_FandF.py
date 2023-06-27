import os.path
import unittest
from typing import Dict

import utils
from parse_table_gen.ebnf_parser import parse_ebnf_file
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.consts import EMPTY

class TestFirstAndFollow(unittest.TestCase):

    def _checkFirstSet(self, expected: Dict[str, set[str]], actual: Dict[str, set[str]]):
        for key, val in actual.items():
            self.assertTrue(key in expected, f'Symbol "{key}" not in epected First dict')
            expSet = expected[key]
            diff = expSet ^ val
            self.assertEqual(0, len(diff), f'Bad first set for symbol: "{key}", expected: {expSet} got {val}')

        self.assertEqual(len(expected), len(actual), 'Len of first set not equal to expected')


    def test_FandF1(self):
        """
        From compiler book by Thain
        """
        testFile = utils.getTestFilename("FandFtest1.ebnf")
        grammer = parse_ebnf_file(testFile)
        ff = FirstAndFollow(grammer)

        EXP_FIRST = {
            'P': {'open_p', 'int'},
            'E': {'open_p', 'int'},
            'EP': {'plus', EMPTY},
            'T': {'open_p', 'int'},
            'TP': {'star', EMPTY},
            'F': {'open_p', 'int'},
            'plus': {'plus'},
            'star': {'star'},
            'open_p': {'open_p'},
            'close_p': {'close_p'},
            'int': {'int'}
        }


        self._checkFirstSet(EXP_FIRST, ff.first)

        EXP_FOLLOW = {
            'P': {'$'},
            'E': {'close_p', '$'},
            'EP': {'close_p', '$'},
            'T': {'close_p', 'plus', '$'},
            'TP': {'plus', 'close_p', '$'},
            'F': {'plus', 'star', 'close_p', '$'}
        }

        """
        for key, val in ff.follow.items():
            self.assertTrue(key in EXP_FOLLOW, 'Name not in epected Follow dict')
            expSet = EXP_FOLLOW[key]
            self.assertEqual(len(expSet), len(val), 'Size of Follow set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected Follow set')
        """
        
    
    def testFandF2(self):
        """
        Test from https://people.cs.pitt.edu/~jmisurda/teaching/cs1622/handouts/cs1622-first_and_follow.pdf
        """
        testFile = utils.getTestFilename('FandFtest2.ebnf')
        g = parse_ebnf_file(testFile)

        ff = FirstAndFollow(g)

        EXP_FIRST = {
            'Y': {'star', EMPTY},
            'X': {'plus', EMPTY},
            'T': {'open_p', 'int'},
            'E': {'open_p', 'int'},
            'plus': {'plus'},
            'star': {'star'},
            'open_p': {'open_p'},
            'close_p': {'close_p'},
            'int': {'int'}
        }

        self._checkFirstSet(EXP_FIRST, ff.first)

        EXP_FOLLOW = {
            'Y': {'close_p', '$', 'plus'},
            'X': {'close_p', '$'},
            'T': {'close_p', '$', 'plus'},
            'E': {'close_p', '$'}
        }

        """
        for key, val in ff.follow.items():
            self.assertTrue(key in EXP_FOLLOW, 'Name not in epected Follow dict')
            expSet = EXP_FOLLOW[key]
            self.assertEqual(len(expSet), len(val), 'Size of Follow set not equal')
            for x in val:
                self.assertTrue(x in expSet, 'Value not in expected Follow set')
        """