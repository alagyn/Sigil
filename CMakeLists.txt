cmake_minimum_required(VERSION 3.26)

project(Sigil)

set(HERMES_GRAMMAR ${CMAKE_SOURCE_DIR}/lang_descr/sigil.hm)
set(HERMES_ADDITIONAL_INCLUDES ${CMAKE_SOURCE_DIR}/sigil/sigil-ast/inc)

find_package(Boost 1.82.0 
    REQUIRED
    COMPONENTS program_options filesystem
)

add_subdirectory(deps)

add_subdirectory(sigil)
