project("LeapOfFaithEngine")

add_executable(${PROJECT_NAME} "src/main.cpp")

target_link_libraries(${PROJECT_NAME} PUBLIC LeapOfFaithLib)

# install(FILES config.json DESTINATION bin)
file(COPY config.json DESTINATION ${CMAKE_BINARY_DIR}/bin)
