cmake_minimum_required (VERSION 2.6)
project (conway_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ggdb")
endif()

set(SNAKE_APP_SOURCES
    conway_main.c
)

add_library(conway_app ${SNAKE_APP_SOURCES})
set_property(TARGET conway_app PROPERTY C_STANDARD 99)
