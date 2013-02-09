find_package(OpenGL REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLEW REQUIRED)

set(LIBRARIES ${GLFW_LIBRARY} ${GLEW_LIBRARY} ${OPENGL_LIBRARY})

add_executable(mai_first_play main.cpp shader.cpp)

target_link_libraries(mai_first_play ${LIBRARIES})
