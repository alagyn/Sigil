import unittest

import utils
from parse_table_gen.main import parse_ebnf_file
from parse_table_gen.ebnf_grammer import Grammer
from parse_table_gen.first_and_follow import FirstAndFollow
from parse_table_gen.lalr1_automata import LALR1Automata, ParseTable


class TestParseTable(unittest.TestCase):

    def test_table(self):
        testFile = utils.getTestFilename("G10.sebnf")

        grammer = parse_ebnf_file(testFile)
        ff = FirstAndFollow(grammer)
        lalr = LALR1Automata(grammer, ff)
        table = ParseTable(lalr)

        # symbol order
        # E T id plus open_p close_p END

        # yapf: disable
        EXP_TABLE = [
            [(ParseTable.ACCEPT, 0), (ParseTable.GOTO, 1),  (ParseTable.GOTO, 2),  (ParseTable.SHIFT, 3), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.SHIFT, 4),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0),  (ParseTable.REDUCE, 1)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 3), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 3), (ParseTable.REDUCE, 3)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 5), (ParseTable.SHIFT, 5), (ParseTable.REDUCE, 5), (ParseTable.REDUCE, 5)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.GOTO, 6),  (ParseTable.SHIFT, 3), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0)],
            [(ParseTable.ERROR, 0),  (ParseTable.GOTO, 7),  (ParseTable.GOTO, 2),  (ParseTable.SHIFT, 3), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0),  (ParseTable.ERROR, 0)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 2), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 2), (ParseTable.REDUCE, 2)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.SHIFT, 4),  (ParseTable.ERROR, 0), (ParseTable.SHIFT, 8),  (ParseTable.ERROR, 0)],
            [(ParseTable.ERROR, 0),  (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 4), (ParseTable.ERROR, 0), (ParseTable.REDUCE, 4), (ParseTable.REDUCE, 4)]
        ]
        # yapf: enable

        for rowIdx, (expRow, actRow) in enumerate(zip(EXP_TABLE, table.table)):
            for colIdx, (expCol, actCol) in enumerate(zip(expRow, actRow)):
                self.assertEqual(expCol, actCol, f"Error at [{rowIdx}][{colIdx}] expected {expCol} got {actCol}")

            self.assertEqual(len(expRow), len(actRow), f"Row {rowIdx} not of equal length")
        self.assertEqual(len(EXP_TABLE), len(table.table), f"Table not of equal size")