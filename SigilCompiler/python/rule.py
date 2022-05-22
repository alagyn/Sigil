
from typing import List

class _RuleIter:
    def __init__(self, s: List[str]):
        self._idx = -1
        self._x = s

    def __next__(self):
        self._idx += 1
        try:
            return self._x[self._idx]
        except IndexError:
            raise StopIteration

class Rule:
    def __init__(self, nonterm: str, r: List[str]):
        self.nonterm = nonterm
        self.symbols = r

        x = nonterm + "".join(r)
        self._hash = hash(x)


    def __eq__(self, other: 'Rule'):
        if not isinstance(other, Rule):
            return False

        for x, y in zip(self.symbols, other.symbols):
            if x != y:
                return False

        return self.nonterm == other.nonterm

    def __iter__(self):
        return _RuleIter(self.symbols)

    def __ne__(self, other):
        return not self.__eq__(other)

    def __str__(self):
        r = ' '.join(x for x in self.symbols)
        return f'[{self.nonterm} = {r}]'

    def __hash__(self):
        return self._hash

    def compare(self, other) -> int:
        if not isinstance(other, Rule):
            raise NotImplementedError(f'Cannot compare Rule to {type(other)}')

        snt = self.nonterm.lower()
        ont = other.nonterm.lower()

        if snt < ont:
            return -1
        elif snt > ont:
            return 1

        if len(self.symbols) < len(other.symbols):
            return -1
        if len(self.symbols) > len(other.symbols):
            return 1

        for x, y in zip(self.symbols, other.symbols):
            x = x.lower()
            y = y.lower()
            if x < y:
                return -1
            if x > y:
                return 1

        return 0

    def __lt__(self, other):
        return self.compare(other) == -1

    def __le__(self, other):
        return self.compare(other) <= 0

    def __gt__(self, other):
        return self.compare(other) == 1

    def __ge__(self, other):
        return self.compare(other) >= 0
