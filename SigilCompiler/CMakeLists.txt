
project(sgc)

find_package(Boost 1.82.0 
    REQUIRED
    COMPONENTS program_options regex filesystem
)

set(SGC_SRC
    src/main.cpp
    src/scanner.cpp
)

add_executable(${PROJECT_NAME} ${SGC_SRC})

target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Boost::program_options
        Boost::regex
        Boost::filesystem
)

add_dependencies(${PROJECT_NAME} ParseTableGen)

