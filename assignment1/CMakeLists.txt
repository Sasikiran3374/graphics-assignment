cmake_minimum_required(VERSION 3.16)

project(assignment1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(assignment1
    src/main.cpp
    src/window.cpp
    src/shader.cpp
)

target_include_directories(assignment1 PRIVATE
    ${PROJECT_SOURCE_DIR}
    /Users/sasi/Desktop
)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)

target_link_libraries(assignment1 PRIVATE
    OpenGL::GL
    glfw
    GLEW::GLEW
)