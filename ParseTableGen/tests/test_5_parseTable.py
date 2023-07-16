import unittest

import utils
from parse_table_gen.main import parse_ebnf_file
from parse_table_gen.ebnf_grammer import Grammer
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.lalr1_automata import LALR1Automata, ParseTable, Action, TableType, ParseAction

PA = ParseAction


class TestParseTable(unittest.TestCase):

    def _checkTable(self, exp: TableType, act: TableType):
        for rowIdx, (expRow, actRow) in enumerate(zip(exp, act)):
            for colIdx, (expCol, actCol) in enumerate(zip(expRow, actRow)):
                self.assertEqual(expCol, actCol, f"Error at [{rowIdx}][{colIdx}] expected {expCol} got {actCol}")

            self.assertEqual(len(expRow), len(actRow), f"Row {rowIdx} not of equal length")
        self.assertEqual(len(exp), len(act), f"Table not of equal size")

    def test_1_G10(self):
        testFile = utils.getTestFilename("G10.sebnf")
        grammer = parse_ebnf_file(testFile)
        ff = FirstAndFollow(grammer)
        lalr = LALR1Automata(grammer, ff)
        table = ParseTable(lalr)

        # symbol order
        # P E T id plus open_p close_p END

        # yapf: disable
        EXP_TABLE: TableType = [
            [PA(Action.G, 1), PA(Action.G, 2), PA(Action.S, 3), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.S, 4), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 2), PA(Action.E, 0), PA(Action.R, 2), PA(Action.R, 2)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 4), PA(Action.S, 5), PA(Action.R, 4), PA(Action.R, 4)],
            [PA(Action.E, 0), PA(Action.G, 6), PA(Action.S, 3), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0)],
            [PA(Action.G, 7), PA(Action.G, 2), PA(Action.S, 3), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 1), PA(Action.E, 0), PA(Action.R, 1), PA(Action.R, 1)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.S, 4), PA(Action.E, 0), PA(Action.S, 8), PA(Action.E, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 3), PA(Action.E, 0), PA(Action.R, 3), PA(Action.R, 3)]
        ]
        # yapf: enable

        self._checkTable(EXP_TABLE, table.table)

    def test_2_epsilon(self):
        testFile = utils.getTestFilename("epsilon.sebnf")
        grammer = parse_ebnf_file(testFile)
        ff = FirstAndFollow(grammer)
        lalr = LALR1Automata(grammer, ff)
        table = ParseTable(lalr)

        print(table.symbolList)

        # symbol order S A B b a END

        # yapf: disable
        EXP_TABLE: TableType = [
            [PA(Action.G, 1), PA(Action.G, 2), PA(Action.R, 3), PA(Action.R, 3), PA(Action.E, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.S, 3), PA(Action.S, 4), PA(Action.E, 0)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 1)],
            [PA(Action.E, 0), PA(Action.E, 0), PA(Action.R, 2), PA(Action.R, 2), PA(Action.E, 0)]
        ]
        # yapf: enable

        self._checkTable(EXP_TABLE, table.table)