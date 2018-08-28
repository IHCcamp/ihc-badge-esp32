cmake_minimum_required (VERSION 2.6)

# Apps
add_subdirectory(components/conway_app)
add_subdirectory(components/messages_app)
add_subdirectory(components/interp_app)
add_subdirectory(components/phonebook_app)
add_subdirectory(components/settings_app)
add_subdirectory(components/games_app)
add_subdirectory(components/multimedia_app)
add_subdirectory(components/snake_app)
add_subdirectory(components/about_app)

# Libraries
add_subdirectory(components/ui)

# Misc
add_subdirectory(components/shell)
add_subdirectory(Linux)
