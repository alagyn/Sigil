cmake_minimum_required(VERSION 3.26)

project(Sigil)

set(GRAMMER_FILE "${CMAKE_SOURCE_DIR}/lang_descr/sigil.sebnf" CACHE FILEPATH "The grammer file")

add_subdirectory(ParseTableGen)
add_subdirectory(SigilCompiler)
