

project(sigil_ast)

set(${PROJECT_NAME}_SRC
    src/syntaxTree.cpp
)

add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_SRC})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc
)