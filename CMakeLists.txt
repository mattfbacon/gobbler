cmake_minimum_required(VERSION 3.21)

set(EXECUTABLE_NAME gobbler)
set(PROJECT_NAME "Gobbler Argument Parser")

# Dirs
set(DIR_SRC src)
set(DIR_DIST dist)
set(DIR_INCLUDE include)
set(DIR_SYSINC sysinc)
set(DIR_LIB lib)

project(${PROJECT_NAME} VERSION 0.1 LANGUAGES C CXX)

# Sources
set (sources
	ArgParser.cpp
	)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_POSITION_INDEPENDENT_CODE True)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_CXX_STANDARD_REQUIRED True)

list(TRANSFORM sources PREPEND ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_SRC}/)

add_library(${EXECUTABLE_NAME}-shared EXCLUDE_FROM_ALL SHARED ${sources})
add_library(${EXECUTABLE_NAME}-static EXCLUDE_FROM_ALL STATIC ${sources})
add_library(${EXECUTABLE_NAME}-object EXCLUDE_FROM_ALL OBJECT ${sources})

foreach(target ${EXECUTABLE_NAME}-shared ${EXECUTABLE_NAME}-static ${EXECUTABLE_NAME}-object)

set_target_properties(${target} PROPERTIES C_STANDARD 17 CXX_STANDARD 20)

target_include_directories(${target} PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_INCLUDE})
target_include_directories(${target} SYSTEM INTERFACE ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_INCLUDE})
target_include_directories(${target} SYSTEM PRIVATE ${${PROJECT_NAME}_SOURCE_DIR}/${DIR_SYSINC})

set_property(TARGET ${target} PROPERTY RUNTIME_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET ${target} PROPERTY LIBRARY_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")
set_property(TARGET ${target} PROPERTY ARCHIVE_OUTPUT_DIRECTORY "${${PROJECT_NAME}_SOURCE_DIR}/${DIR_DIST}")

target_compile_options(${target} PRIVATE
-Wall -Wextra
-Wno-unknown-pragmas
-Wcpp -Wshadow -Wundef
-fno-common -Wconversion
-Wno-enum-compare
-frtti -fexceptions

"$<$<CXX_COMPILER_ID:GNU>:\
-fdiagnostics-color\
>"

"$<$<CXX_COMPILER_ID:Clang>:\
-fcolor-diagnostics;\
-Wno-gnu-zero-variadic-macro-arguments;\
-Wno-c++11-extensions;-Wno-c++14-extensions;-Wno-c++17-extensions;-Wno-c++20-extensions;\
-Wno-unused-command-line-argument\
>"

"$<$<CONFIG:Debug>:\
-fno-omit-frame-pointer;\
-fstack-protector-all;\
-fstack-clash-protection;\
-O0;\
-ggdb3;\
-Werror;\
-Wfatal-errors;\
-Wno-error=cpp;\
-Wno-error=pedantic;\
-fasynchronous-unwind-tables;\
-finstrument-functions;\
-fsanitize=address;\
-fsanitize=undefined;\
-fsanitize=leak\
>")

target_compile_definitions(${target} PRIVATE
"MSGPACK_DISABLE_LEGACY_CONVERT"

"$<$<CONFIG:Release>:\
NDEBUG\
>"

"$<$<CONFIG:Debug>:\
_GLIBCXX_DEBUG;\
_GLIBCXX_ASSERTIONS\
>")

target_link_options(${target} PRIVATE
"-fuse-ld=lld"
)

target_link_options(${target} PUBLIC
"-Wl,-z,defs,-z,now,-z,relro"

"$<$<CONFIG:Debug>:\
-fsanitize=address;\
-fsanitize=undefined;\
-fsanitize=leak\
>")

set_target_properties(${target} PROPERTIES OUTPUT_NAME ${EXECUTABLE_NAME})

endforeach()

include(test.cmake)

# Extra Stuff Starts Here
