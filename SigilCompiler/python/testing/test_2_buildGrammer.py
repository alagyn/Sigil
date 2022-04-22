import unittest
import os.path

from testConsts import TEST_ROOT
import ebnf_parser
from grammer import Grammer
from rule import Rule

class TestBuildGrammer(unittest.TestCase):

    def test_buildGrammer1(self):
        testFile = os.path.join(TEST_ROOT, 'test.ebnf')

        g = ebnf_parser.parseEBNFFile(testFile)

        self.assertEqual('PROGRAM', g.goalNT, 'The goal state is incorrect')

        EXP_TERMS = {
            'semicolon': ';',
            'open_curly': "{",
            'close_curly': "}",
            'open_paren': "(",
            'close_paren': ")",
            'equals_sign': "=",
            'name': "[a-zA-Z][a-zA-Z0-9_]*",
            'integer': '[1-9][0-9]*',
            '$': '$'
        }

        self.assertEqual(len(EXP_TERMS), len(g.terminals), 'Len of terminal definitions not equal')

        for key, val in g.terminals.items():
            self.assertTrue(key in EXP_TERMS, 'Terminal name is not expected')
            self.assertEqual(EXP_TERMS[key], val, 'Terminal definition is not expected')

        EXP_RULES = {
            'PROGRAM': [
                Rule('PROGRAM', ['stmt'])
            ],
            'stmt': [
                Rule('stmt', ['name', 'equals_sign', 'integer', 'semicolon']),
                Rule('stmt', ['open_curly', 'integer', 'close_curly'])
            ]
        }

        self.assertEqual(len(EXP_RULES), len(g.rules), 'Len of rules not equal')

        for key, val in g.rules.items():
            self.assertTrue(key in EXP_RULES, 'Rule name is not expected')
            for exp, act in zip(EXP_RULES[key], val):
                self.assertEqual(exp, act, 'Rule is not as expected')


    # TODO invalid test files
