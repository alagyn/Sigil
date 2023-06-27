cmake_minimum_required(VERSION 3.23)

project(SigilCompiler LANGUAGES CXX)

add_subdirectory(src src)

set(CMAKE_BUILD_CONFIGURATION_TYPES Debug Release CACHE STRING "The build type" FORCE)
set(CMAKE_CONFIGURATION_TYPES Debug Release CACHE STRING "The build type" FORCE)
set(CMAKE_BUILD_TYPE Debug CACHE STRING "The build type" FORCE)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE STRING "Export the commands for clangd" FORCE)
set(PYTHON_PATH "$ENV{LOCALAPPDATA}/Programs/Python/Python39" CACHE PATH "Path to python directory" FORCE)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin FORCE)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_BINARY_DIR}/bin FORCE)


# Util  Functions
function(print)
    foreach(var ${ARGN})
        message("${var} = ${${var}}")
    endforeach()
endfunction()

function(print_env)
    foreach(var ${ARGN})
        message("${var} = $ENV{${var}}")
    endforeach()
endfunction()
