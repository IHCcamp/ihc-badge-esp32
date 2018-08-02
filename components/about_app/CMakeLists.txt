cmake_minimum_required (VERSION 2.6)
project (about_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(ABOUT_APP_SOURCES
    about_main.c
)

add_library(about_app ${ABOUT_APP_SOURCES})
set_property(TARGET about_app PROPERTY C_STANDARD 99)
