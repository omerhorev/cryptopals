cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(cryptopals VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_FILES * ../include/*)

include_directories(../include)

add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ../contrib/Catch2/single_include)

add_executable(cryptopals-tests ${SRC_FILES})
target_link_libraries(cryptopals-tests Catch)
