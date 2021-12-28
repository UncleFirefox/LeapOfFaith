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

find_package(Vulkan REQUIRED)

# find_package(Vulkan REQUIRED FATAL_ERROR)
# FindVulkan could be a good candidate to change this to
target_link_libraries(${PROJECT_NAME} ${CONAN_PACKAGES} Vulkan::Vulkan Vulkan::Headers)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>)