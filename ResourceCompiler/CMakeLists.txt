project("ResourceCompiler")

set(SRC src/main.cpp src/MeshCompiler.cpp src/MeshCompiler.h)
add_executable(${PROJECT_NAME} ${SRC})

set(CONAN_PACKAGES CONAN_PKG::assimp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CONAN_PACKAGES})
target_link_libraries(${PROJECT_NAME} PUBLIC LeapOfFaithLib)