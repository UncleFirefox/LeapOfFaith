cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(LeapOfFaith VERSION 0.1.0 LANGUAGES CXX)

# Generating compile commands file, very useful for tools like clangd
# https://stackoverflow.com/questions/68693127/are-all-cmake-variables-automatically-defined-in-cmake-esp-cmake-export-compil/68724763#68724763
if(NOT DEFINED ENV{CMAKE_EXPORT_COMPILE_COMMANDS})
    set($ENV{CMAKE_EXPORT_COMPILE_COMMANDS} TRUE)
endif()

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory("LeapOfFaithLib")
add_subdirectory("ResourceCompiler")
add_subdirectory("LeapOfFaithEngine")

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_BINARY_DIR}/bin)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/models DESTINATION ${CMAKE_BINARY_DIR}/bin)