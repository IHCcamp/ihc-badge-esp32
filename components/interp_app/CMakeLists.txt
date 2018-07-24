cmake_minimum_required (VERSION 2.6)
project (interp_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(INTERP_APP_SOURCES
    interp_main.c
)

add_library(interp_app ${INTERP_APP_SOURCES})
set_property(TARGET interp_app PROPERTY C_STANDARD 99)
