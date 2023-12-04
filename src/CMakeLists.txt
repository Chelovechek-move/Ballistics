set(MODULE_NAME ballistics)
message("ballistics " ${MODULE_NAME})

file(GLOB_RECURSE SOURCES *.h *.cpp *.hpp)
add_library(${MODULE_NAME} STATIC ${SOURCES})

set(TARGET_LIBS Eigen sofa calceph GeographicLib)

set_target_properties(${MODULE_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(${MODULE_NAME} PUBLIC ${TARGET_LIBS})
target_include_directories(${MODULE_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})