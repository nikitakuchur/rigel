cmake_minimum_required(VERSION 3.16)
project(rigel)

set(CMAKE_CXX_STANDARD 17)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

file(GLOB RIGEL_SOURCES src/*.cpp src/render/*.cpp src/window/*.cpp)
add_library(rigel SHARED ${RIGEL_SOURCES})
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

target_include_directories(rigel PUBLIC deps/stb_image/include)
