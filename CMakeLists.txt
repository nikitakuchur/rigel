cmake_minimum_required(VERSION 3.16)
project(rigel)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB RIGEL_SOURCES src/*.cpp src/glutils/*.cpp)
add_library(rigel SHARED ${RIGEL_SOURCES} src/OrthographicCamera.cpp src/OrthographicCamera.h src/Camera.cpp src/Camera.h)
target_include_directories(rigel PUBLIC src/)

add_subdirectory(deps/glad)
target_link_libraries(rigel glad)
target_include_directories(rigel PUBLIC glad/include)

add_subdirectory(deps/glfw)
target_link_libraries(rigel glfw)
target_include_directories(rigel PUBLIC glfw/include)
find_package(OpenGL REQUIRED)
target_link_libraries(rigel OpenGL::GL)

target_include_directories(rigel PUBLIC deps/glm)