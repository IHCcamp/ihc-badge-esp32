cmake_minimum_required (VERSION 2.6)
project (multimedia_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(MULTIMEDIA_APP_SOURCES
    multimedia_main.c
)

add_library(multimedia_app ${MULTIMEDIA_APP_SOURCES})
set_property(TARGET multimedia_app PROPERTY C_STANDARD 99)
