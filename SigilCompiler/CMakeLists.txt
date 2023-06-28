cmake_minimum_required(VERSION 3.26)

project(Sigil LANGUAGES CXX)

set(SGC_SRC
    src/main.cpp
    src/scanner.cpp
)

add_executable(sgc ${SGC_SRC})

target_include_directories(sgc
    PUBLIC
        "include"
)

