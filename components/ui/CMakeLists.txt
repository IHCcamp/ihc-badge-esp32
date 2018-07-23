cmake_minimum_required (VERSION 2.6)
project (ui)

include_directories(
    include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(UI_SOURCES
    input.c
    painter.c
    ui.c
)

add_library(ui ${UI_SOURCES})
set_property(TARGET ui PROPERTY C_STANDARD 99)
