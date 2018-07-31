cmake_minimum_required (VERSION 2.6)
project (games_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(GAMES_APP_SOURCES
    games_main.c
)

add_library(games_app ${GAMES_APP_SOURCES})
set_property(TARGET games_app PROPERTY C_STANDARD 99)
