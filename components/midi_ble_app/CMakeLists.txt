cmake_minimum_required (VERSION 2.6)
project (midi_ble_app)

include_directories(
    include
    ../ui/include
    ../hwcontext/include
)

if(CMAKE_COMPILER_IS_GNUCC)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -ggdb")
endif()

set(MIDI_BLE_SOURCES
    midi_ble_main.c
)

add_library(midi_ble_app ${MIDI_BLE_SOURCES})
set_property(TARGET midi_ble_app PROPERTY C_STANDARD 99)
