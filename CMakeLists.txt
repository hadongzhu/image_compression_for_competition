cmake_minimum_required(VERSION 3.23)
project(image_compression_for_competition)

set(CMAKE_CXX_STANDARD 23)

include_directories(./src)
aux_source_directory(./src SRCS)
add_executable(image_compression_for_competition ./src/main.cpp ${SRCS})
