project(ParseTableGen)

find_package(Python3 
    REQUIRED 
    COMPONENTS Interpreter
)

#[[
    Automatically regenerates ParseTable.h
    whenever the python code or the chosen grammer file changes
]]#

set(OUTPUT_HEADER ${CMAKE_SOURCE_DIR}/SigilCompiler/inc/ParseTable.h)

set(PY_FILES
    parse_table_gen/consts.py
    parse_table_gen/ebnf_grammer.py
    parse_table_gen/first_and_follow.py
    parse_table_gen/lalr1_automata.py
    parse_table_gen/main.py
    parse_table_gen/preprocessing.py
)

# Custom command to generate the header
add_custom_command(
    OUTPUT ${OUTPUT_HEADER}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND ${Python3_EXECUTABLE} -m parse_table_gen.main ${GRAMMER_FILE} ${OUTPUT_HEADER}
    DEPENDS ${GRAMMER_FILE} ${PY_FILES}
)

# Custom target that depends on the output so that we can specify it as a dependency
add_custom_target(${PROJECT_NAME}
    DEPENDS ${OUTPUT_HEADER} ${GRAMMER_FILE} ${PY_FILES}
)