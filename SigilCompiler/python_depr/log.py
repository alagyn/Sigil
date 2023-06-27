_ERR_C = '\033[91m'
_DBG_C = '\033[92m'
_WRN_C = '\033[93m'
_END_C = '\033[0m'

HEADER = " EBNF :"

# TODO update logging

_ENABLED = True


def enableLogging(b):
    global _ENABLED
    _ENABLED = b


def logInfo(m):
    if _ENABLED:
        print(f'INFO :' + HEADER, m)


def logWrn(m):
    if _ENABLED:
        print(_WRN_C + "WARN :" + HEADER + m + _END_C)


def logErr(m):
    if _ENABLED:
        print(_ERR_C + "ERROR:" + HEADER + m + _END_C)
