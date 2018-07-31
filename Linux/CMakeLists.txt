cmake_minimum_required (VERSION 2.6)
project (ihc-badge-linux)

find_package(SDL REQUIRED)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -pedantic -Wextra -ggdb")
endif()

include_directories(
    ${SDL_INCLUDE_DIR}

    include
    ../components/hwcontext/include
)

set(LINUX_SOURCES
    main.c
    hwcontext.c
)

add_executable(ihc-badge-linux ${LINUX_SOURCES})

target_link_libraries(ihc-badge-linux
    # Apps
    settings_app
    games_app
    snake_app

    # Libs
    ui

    # Misc
    shell
    ${SDL_LIBRARY}
)
