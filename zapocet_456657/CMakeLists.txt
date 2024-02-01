# ADD SOURCE FILES HERE
add_executable(zapocet_456657 main.cpp Application.cpp Application.hpp)

# ADD libraries which are imported through CMake(ExternalProjectAdd or otherwise) here
add_dependencies(zapocet_456657 glfw framework)
target_link_libraries(zapocet_456657 ${GLFW_LIBRARIES} ${GLAD_LIBRARIES} framework)

# ADD includes of libraries here
target_include_directories(zapocet_456657
	PRIVATE ${GLFW_INCLUDE_DIR}
	PRIVATE ${GLAD_INCLUDE_DIR}
	PRIVATE ${SINGLE_HEADER_LIBS_INCLUDE_DIR}
	PRIVATE ${FRAMEWORK_INCLUDE_DIR}
)

# USE configure_file to copy your shaders, images, obj files to build folder so that they can be found using relative paths
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/main.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/main.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/lights.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/lights.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/lights.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/lights.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/skybox.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/skybox.frag" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/exterior.vert" "${CMAKE_CURRENT_BINARY_DIR}/shaders/exterior.vert" COPYONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/shaders/exterior.frag" "${CMAKE_CURRENT_BINARY_DIR}/shaders/exterior.frag" COPYONLY)
file(COPY "${OBJ_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(COPY "${IMAGE_DIR}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
