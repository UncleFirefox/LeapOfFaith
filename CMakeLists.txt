cmake_minimum_required(VERSION 3.22 FATAL_ERROR)
project(LeapOfFaith VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup(TARGETS)

add_subdirectory("LeapOfFaithLib")
add_subdirectory("ResourceCompiler")
add_subdirectory("LeapOfFaithEngine")