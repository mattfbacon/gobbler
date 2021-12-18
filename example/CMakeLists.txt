cmake_minimum_required(VERSION 3.0)
project(gobbler-example CXX)

add_subdirectory(gobbler)

add_executable(example main.cpp)
target_link_libraries(example PRIVATE gobbler-static)
set_target_properties(example PROPERTIES C_STANDARD 17 CXX_STANDARD 20)

set(DIR_DIST dist)
set_property(TARGET example PROPERTY RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET example PROPERTY LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET example PROPERTY ARCHIVE_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
