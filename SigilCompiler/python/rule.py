
from typing import List

class Rule:
    def __init__(self, nonterm: str, r: List[str]):
        self.nonterm = nonterm
        self.rule = r

    def __eq__(self, other: 'Rule'):
        for x, y in zip(self.rule, other.rule):
            if x != y:
                return False

        return self.nonterm == other.nonterm

    def __str__(self):
        r = ' '.join(x for x in self.rule)
        return f'[{self.nonterm} = {r}]'