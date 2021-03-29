cmake_minimum_required(VERSION 3.16)
project(jb_triangle)

set(CMAKE_CXX_STANDARD 14)

file(GLOB_RECURSE SRCS src/*.cpp)

add_executable(jb_triangle ${SRCS})


find_package(X11 REQUIRED)
find_package(OpenGL)
target_link_libraries(jb_triangle PUBLIC OpenGL::GL)
target_link_libraries(jb_triangle PUBLIC X11)