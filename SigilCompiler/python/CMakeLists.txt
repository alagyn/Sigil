
set(SIGIL_PY_FILES
    ebnf_parser.py
)

set(SIGIL_PY "")

foreach(var ${SIGIL_PY_FILES})
    list(APPEND SIGIL_PY "${CMAKE_CURRENT_SOURCE_DIR}/${var}")
endforeach()

set(SIGIL_PY ${SIGIL_PY} PARENT_SCOPE)