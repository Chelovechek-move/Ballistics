file(GLOB_RECURSE SOURCES *.h *.cpp *.hpp *.c *.h.in)

add_library(GeographicLib STATIC ${SOURCES})
message(GeographicLib include dir ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(GeographicLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})