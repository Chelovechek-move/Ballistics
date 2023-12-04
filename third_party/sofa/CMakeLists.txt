file(GLOB_RECURSE SOURCES *.h *.cpp *.hpp *.c)

add_library(sofa STATIC ${SOURCES})
message(sofa include dir ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(sofa PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})