cmake_minimum_required(VERSION 3.23)
project(image_compression_for_competition)

set(CMAKE_CXX_STANDARD 23)

include_directories(./src)
aux_source_directory(./src SRC)
aux_source_directory(./jpeg-ls JPEG_LS)
list(REMOVE_ITEM JPEG_LS "./jpeg-ls/main.c")
aux_source_directory(./clzip CLZIP)
#list(REMOVE_ITEM CLZIP "./clzip/main.c")
add_definitions(-DPROGVERSION="1.13")
add_executable(image_compression_for_competition ${SRC} ${JPEG_LS} ${CLZIP})
