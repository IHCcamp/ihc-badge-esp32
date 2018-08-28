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
    ../components/ui/include
)

set(LINUX_SOURCES
    main.c
    hwcontext.c
    midi_ble_main.c
)

add_executable(ihc-badge-linux ${LINUX_SOURCES})

target_link_libraries(ihc-badge-linux
    # Apps
    settings_app
    games_app
    multimedia_app
    snake_app

    # Libs
    ui

    # Misc
    shell
    ${SDL_LIBRARY}
)
