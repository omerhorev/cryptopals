cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(data-structure-task VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE SRC_FILES * ../include/*)

include_directories(../include)

add_executable(cryptopals ${SRC_FILES})