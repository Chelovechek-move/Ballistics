cmake_minimum_required(VERSION 3.16)
project(project_ballistics)
set(CMAKE_CXX_STANDARD 17)

add_subdirectory(sofa)
add_subdirectory(calceph)
add_subdirectory(GeographicLib)
add_library(Eigen INTERFACE)
target_include_directories(Eigen INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

#add_library(${LIB_NAME} STATIC ${SOURCES})
#set_target_properties(${LIB_NAME} PROPERTIES LINKER_LANGUAGE CXX)