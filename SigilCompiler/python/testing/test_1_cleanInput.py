import unittest
import os.path

from testConsts import TEST_ROOT

import grammer
import ebnf_parser

EXPECTED_CLEAN = [
        'semicolon = ";";',
        'open_curly = "{";',
        'close_curly = "}";',
        'open_paren = "(";',
        'close_paren = ")";',
        'equals_sign = "=";',
        'name = "[a-zA-Z][a-zA-Z0-9_]*";',
        "integer = '[1-9][0-9]*';",
        '@PROGRAM;',
        'PROGRAM = stmt;',
        'stmt = name equals_sign integer',
        'semicolon;',
        'stmt = open_curly integer close_curly;'
    ]

EXPECTED_COMBINED = [
'semicolon = ";";',
        'open_curly = "{";',
        'close_curly = "}";',
        'open_paren = "(";',
        'close_paren = ")";',
        'equals_sign = "=";',
        'name = "[a-zA-Z][a-zA-Z0-9_]*";',
        "integer = '[1-9][0-9]*';",
        '@PROGRAM;',
        'PROGRAM = stmt;',
        'stmt = name equals_sign integer semicolon;',
        'stmt = open_curly integer close_curly;'
]

class TestFileClean(unittest.TestCase):
    def test_parse(self):
        testFile = os.path.join(TEST_ROOT, 'test.ebnf')

        cleanLines = ebnf_parser.cleanInput(testFile)

        self.assertEqual(len(EXPECTED_CLEAN), len(cleanLines), 'The length of the clean lines')

        for exp, act in zip(EXPECTED_CLEAN, cleanLines):
            self.assertEqual(exp, act)

        combined = ebnf_parser.combineLines(cleanLines)

        self.assertEqual(len(EXPECTED_COMBINED), len(combined), 'The length of the combined lines')

        for exp, act in zip(EXPECTED_COMBINED, combined):
            self.assertEqual(exp, act)