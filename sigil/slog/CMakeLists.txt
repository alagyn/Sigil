
project(slog)

set(${PROJECT_NAME}_SRCS
    src/slog.cpp
)

add_library(${PROJECT_NAME}
    SHARED
        ${${PROJECT_NAME}_SRCS}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        inc
)
