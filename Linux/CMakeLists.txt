cmake_minimum_required (VERSION 2.6)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

find_package(SDL REQUIRED)
include_directories(${SDL_INCLUDE_DIR})

add_executable(ihc-badge-esp32 main.c)
target_link_libraries(ihc-badge-esp32  ${SDL_LIBRARY})
