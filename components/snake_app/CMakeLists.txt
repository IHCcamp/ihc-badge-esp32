cmake_minimum_required (VERSION 2.6)
project (snake_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ggdb")
endif()

set(SNAKE_APP_SOURCES
    snake_main.c
)

add_library(snake_app ${SNAKE_APP_SOURCES})
set_property(TARGET snake_app PROPERTY C_STANDARD 99)
