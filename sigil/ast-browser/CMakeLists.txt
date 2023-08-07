
project(ast-browser)

set(${PROJECT_NAME}_SRCS
    src/ast-browser.cpp
    src/ui.cpp
)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SRCS})

target_include_directories(${PROJECT_NAME} 
    PUBLIC 
        inc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        sigil_ast
        hermes
        Boost::program_options
        Boost::filesystem
        imnodes
        glfw
)