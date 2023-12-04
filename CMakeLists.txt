cmake_minimum_required(VERSION 3.16)
project(project_ballistics)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(src)
add_subdirectory(third_party)
include_directories(third_party)

enable_testing()
add_subdirectory(tests)