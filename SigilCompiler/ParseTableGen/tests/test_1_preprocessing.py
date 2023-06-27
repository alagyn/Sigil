import unittest
import os.path

import parse_table_gen.preprocessing as preprocess

import utils

EXPECTED_CLEAN = [
        'semicolon = ";";',
        'open_curly = "{";',
        'close_curly = "}";',
        'open_paren = "(";',
        'close_paren =   ")";',
        'equals_sign   = "=";',
        'pound = "#";',
        'name = "[a-zA-Z][a-zA-Z0-9_]*";',
        "integer = '[1-9][0-9]*';",
        'PROGRAM   = stmt;',
        'stmt =   name equals_sign integer',
        'semicolon;',
        'stmt = open_curly integer   close_curly;'
    ]

EXPECTED_COMBINED = [
        'semicolon = ";";',
        'open_curly = "{";',
        'close_curly = "}";',
        'open_paren = "(";',
        'close_paren =   ")";',
        'equals_sign   = "=";',
        'pound = "#";',
        'name = "[a-zA-Z][a-zA-Z0-9_]*";',
        "integer = '[1-9][0-9]*';",
        'PROGRAM   = stmt;',
        'stmt =   name equals_sign integer semicolon;',
        'stmt = open_curly integer   close_curly;'
]

class TestPreprocess(unittest.TestCase):
    def test_parse(self):
        testFile = utils.getTestFilename('test.ebnf')

        cleanLines = preprocess.read_and_clean(testFile)

        for exp, act in zip(EXPECTED_CLEAN, cleanLines):
            self.assertEqual(exp, act)

        # Check the length just in case, zip doesn't do this
        self.assertEqual(len(EXPECTED_CLEAN), len(cleanLines), 'The length of the combined lines')

        combined = preprocess.combine_lines(cleanLines)

        for exp, act in zip(EXPECTED_COMBINED, combined):
            self.assertEqual(exp, act)

        self.assertEqual(len(EXPECTED_COMBINED), len(combined), 'The length of the combined lines')