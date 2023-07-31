
project(sgc)


set(SGC_SRC
    src/main.cpp
)

add_executable(${PROJECT_NAME} ${SGC_SRC})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        sigil_ast
        Boost::program_options
        Boost::filesystem
        hermes
)

