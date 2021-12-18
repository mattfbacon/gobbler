set(DIR_TEST test)

set(test_sources
	main.cpp
	)
list(TRANSFORM test_sources PREPEND ${DIR_TEST}/)

add_executable(tests EXCLUDE_FROM_ALL ${test_sources})
target_compile_options(tests PRIVATE -Wall -Wextra)

find_package(Catch2 2 REQUIRED)
target_link_libraries(tests PRIVATE Catch2::Catch2 ${EXECUTABLE_NAME}-static)
set_property(TARGET tests PROPERTY RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET tests PROPERTY LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET tests PROPERTY ARCHIVE_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
