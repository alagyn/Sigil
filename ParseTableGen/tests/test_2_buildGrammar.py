import unittest
import os.path

from parse_table_gen.main import parse_ebnf_file
from parse_table_gen.ebnf_grammer import Rule
from utils import getTestFilename


class TestBuildgrammar(unittest.TestCase):

    def test_buildgrammar1(self):
        testFile = getTestFilename('test.sebnf')

        g = parse_ebnf_file(testFile)

        EXP_TERMS = {
            'semicolon': ';',
            'open_curly': "{",
            'close_curly': "}",
            'open_paren': r"\(",
            'close_paren': r"\)",
            'equals_sign': "=",
            'pound': "#",
            'name': "[a-zA-Z][a-zA-Z0-9_]*",
            'integer': '[1-9][0-9]*',
        }

        self.assertEqual(len(EXP_TERMS), len(g.terminals), 'Len of terminal definitions not equal')

        for key, val in g.terminals.items():
            self.assertTrue(key in EXP_TERMS, 'Terminal name is not expected')
            self.assertEqual(EXP_TERMS[key], val, 'Terminal definition is not expected')

        EXP_RULES = [
            Rule(1, 'PROGRAM', ['stmt']),
            Rule(2, 'stmt', ['name', 'equals_sign', 'integer', 'semicolon']),
            Rule(3, 'stmt', ['open_curly', 'integer', 'close_curly'])
        ]

        self.assertEqual(len(EXP_RULES), len(g.rules), 'Len of rules not equal')

        for exp, act in zip(EXP_RULES, g.rules):
            self.assertEqual(exp, act, "Rule definition is not as expected")

        self.assertEqual("PROGRAM", g.startSymbol, "Start symbol not equal")

    # TODO invalid test files
