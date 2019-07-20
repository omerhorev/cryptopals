cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(cryptopals VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_MODULE_PATH contrib/*)

file(GLOB_RECURSE SRC_FILES src/*.cpp include/*.h)
file(GLOB_RECURSE TEST_FILES tests/*)

add_subdirectory(contrib/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

include_directories(include)
add_executable(ctyptopals-tests ${SRC_FILES} ${TEST_FILES})

target_link_libraries(ctyptopals-tests gtest gtest_main)
add_test(ctyptopals-tests ctyptopals-tests)