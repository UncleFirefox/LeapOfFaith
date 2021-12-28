cmake_minimum_required(VERSION 3.22 FATAL_ERROR)

project("LeapOfFaithLib")
set(SRC src/DataStructures.h
    src/Engine.h src/Engine.cpp
    src/Globals.h src/Globals.cpp
    src/Mesh.h src/Mesh.cpp
    src/MeshModel.h src/MeshModel.cpp
    src/MeshReader.h src/MeshReader.cpp
    src/VulkanRenderer.h src/VulkanRenderer.cpp
    src/Utilities/IO.h
    src/Utilities/Texture.h src/Utilities/Texture.cpp
    src/Utilities/Vulkan.h)

add_library(${PROJECT_NAME} ${SRC})

set(CONAN_PACKAGES CONAN_PKG::glfw CONAN_PKG::glm CONAN_PKG::nlohmann_json CONAN_PKG::stb)

find_package(Vulkan REQUIRED FATAL_ERROR)

# Good reference for cmake spir-v shader compilation: https://pastebin.com/jzk262hg
message("SPIR-V validator path: ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE}")
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/bin/shaders)
execute_process(COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/shader.vert -o ${CMAKE_BINARY_DIR}/bin/shaders/vert.spv)
execute_process(COMMAND ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} -V ${CMAKE_CURRENT_SOURCE_DIR}/src/shaders/shader.frag -o ${CMAKE_BINARY_DIR}/bin/shaders/frag.spv)

# find_package(Vulkan REQUIRED FATAL_ERROR)
# FindVulkan could be a good candidate to change this to
target_link_libraries(${PROJECT_NAME} ${CONAN_PACKAGES} Vulkan::Vulkan Vulkan::Headers)

# Self contained package, nobody needs to know which includes you need using this
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)