import unittest
from typing import Dict

import utils
from parse_table_gen.main import parse_ebnf_file
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.consts import EMPTY, END


class TestFirstAndFollow(unittest.TestCase):

    def _checkSet(self, setType: str, expected: Dict[str, set[str]], actual: Dict[str, set[str]]):
        for key, val in actual.items():
            self.assertTrue(key in expected, f'Symbol "{key}" not in epected {setType} dict')
            expSet = expected[key]
            diff = expSet ^ val
            self.assertEqual(0, len(diff), f'Bad {setType} set for symbol: "{key}", expected: {expSet} got {val}')

        self.assertEqual(len(expected), len(actual), f'Len of {setType} set not equal to expected')

    def test_FandF1(self):
        """
        From compiler book by Thain
        """
        testFile = utils.getTestFilename("FandFtest1.sebnf")
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

        self._checkSet("First", EXP_FIRST, ff.first)

        EXP_FOLLOW = {
            'P': {END},
            'E': {'close_p', END},
            'EP': {'close_p', END},
            'T': {'close_p', 'plus', END},
            'TP': {'plus', 'close_p', END},
            'F': {'plus', 'star', 'close_p', END}
        }

        self._checkSet("Follow", EXP_FOLLOW, ff.follow)

    def testFandF2(self):
        """
        Test from https://people.cs.pitt.edu/~jmisurda/teaching/cs1622/handouts/cs1622-first_and_follow.pdf
        """
        testFile = utils.getTestFilename('FandFtest2.sebnf')
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

        self._checkSet("First", EXP_FIRST, ff.first)

        EXP_FOLLOW = {
            'Y': {'close_p', END, 'plus'}, 'X': {'close_p', END}, 'T': {'close_p', END, 'plus'}, 'E': {'close_p', END}
        }

        self._checkSet("Follow", EXP_FOLLOW, ff.follow)