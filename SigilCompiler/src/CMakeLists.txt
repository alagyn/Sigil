
set(SGC_SRC
    main.cpp
    ebnf_parser.cpp
    sigil_errors.cpp
    sigil_log.cpp
)

add_executable(sgc "${SGC_SRC}")

target_include_directories(sgc
    PUBLIC ${PROJECT_SOURCE_DIR}/include
    PUBLIC ${PYTHON_PATH}/include
)

add_library(python STATIC IMPORTED)
set_target_properties(python PROPERTIES IMPORTED_LOCATION ${PYTHON_PATH}/libs/python39.lib)
add_library(python_d STATIC IMPORTED)
set_target_properties(python_d PROPERTIES IMPORTED_LOCATION ${PYTHON_PATH}/libs/python39_d.lib)
target_link_libraries(sgc python_d)

set(BIN "${CMAKE_SOURCE_DIR}/bin")
set(PYTHON_OUT "/SPython")

# Make python out dir
# Move sgc.exe
add_custom_command(
    TARGET sgc POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${BIN}${PYTHON_OUT}"
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:sgc>
        ${BIN}
)

# Util to move a file to the bin/dir folder
function(move_to_bin binfile dir)
    add_custom_command(
        TARGET sgc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            "${binfile}"
            "${BIN}${dir}"
    )
endfunction()

# foreach python file
foreach(var ${SIGIL_PY})
    # move to bin/python
    move_to_bin("${var}" "${PYTHON_OUT}")
endforeach()

# Move python dll
if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
    move_to_bin(${PYTHON_PATH}/python39_d.dll "")
else()
    move_to_bin(${PYTHON_PATH}/python39.dll "")
endif()
