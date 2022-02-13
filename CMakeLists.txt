cmake_minimum_required(VERSION 3.12)
project(breader)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(src)

# tests
add_subdirectory(extern/googletest)
include(GoogleTest)
include (CTest)
enable_testing()
add_subdirectory(tests)
