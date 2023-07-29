cmake_minimum_required(VERSION 3.26)

project(Sigil)

set(HERMES_GRAMMAR ${CMAKE_SOURCE_DIR}/lang_descr/sigil.hm)
set(HERMES_ADDITIONAL_INCLUDES ${CMAKE_SOURCE_DIR}/SigilCompiler/inc)

add_subdirectory(deps)

add_subdirectory(SigilCompiler)
