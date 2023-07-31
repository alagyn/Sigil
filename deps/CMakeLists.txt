
add_subdirectory(Hermes)

target_link_libraries(hermes
    PUBLIC
        sigil_ast
)

#### GLFW ####
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
add_subdirectory(glfw)


#### Imgui #####
set(imgui_srcs
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp

)

add_library(imgui STATIC ${imgui_srcs})
target_include_directories(imgui
    PUBLIC
        imgui
)

target_link_libraries(imgui
    PRIVATE
    glfw
)

if(${MSVC})
    target_link_libraries(imgui
        PRIVATE
        opengl32.lib
        gdi32.lib
        shell32.lib
    )
else()
    find_package(OpenGL REQUIRED)
    target_link_libraries(imgui
        PRIVATE
        ${OPENGL_LIBRARIES}
    )
endif()


target_compile_definitions(imgui
    PUBLIC
    IMGUI_DISABLE_OBSOLETE_KEYIO
    IMGUI_DISABLE_OBSOLETE_FUNCTIONS
)