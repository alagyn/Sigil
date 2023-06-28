import re
from typing import List

COMMENT_RE = re.compile(r'[^#\'"]*(("[^"]*")|(\'[^\']*\'))?[^#]*#')

def read_and_clean(filename: str) -> List[str]:
    out = []
    with open(filename, mode='r') as f:
        for line in f:
            line = line.strip()
            m = COMMENT_RE.search(line)
            if m is not None and m.end() > 0:
                line = line[0:m.end() - 1].strip()

            if len(line) > 0:
                out.append(line)

    return out

def combine_lines(grammerLines: List[str]) -> List[str]:
    out = []
    curLine = ""
    for line in grammerLines:
        if len(curLine) > 0:
            curLine += " " + line
        else:
            curLine = line

        if curLine.endswith(';'):
            out.append(curLine)
            curLine = ""

    if len(curLine) > 0:
        raise RuntimeError("Grammer does not end with a semicolon")

    return out