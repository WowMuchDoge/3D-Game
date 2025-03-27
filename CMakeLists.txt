cmake_minimum_required(VERSION 3.16)
project(3D-Game)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/include)

add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

add_executable(out 
		src/c/main.c
		src/c/player.c
		src/c/polygon.c)

target_link_libraries(out PRIVATE SDL3::SDL3 m)
