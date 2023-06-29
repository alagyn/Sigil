cmake_minimum_required(VERSION 3.26)

project(Sigil LANGUAGES CXX)

find_package(Boost 1.82.0 
    REQUIRED
    COMPONENTS program_options regex filesystem
)

add_subdirectory(SigilCompiler)
