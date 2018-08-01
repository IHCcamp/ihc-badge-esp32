cmake_minimum_required (VERSION 2.6)
project (shell)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

set(SHELL_SOURCES
    shell_main.c
    shell_menu.c
    apps/apps_list.c
)

add_library(shell ${SHELL_SOURCES})
set_property(TARGET shell PROPERTY C_STANDARD 99)

target_link_libraries(shell
    ui

    # Apps
    messages_app
    interp_app
    settings_app
    games_app
    snake_app
)

