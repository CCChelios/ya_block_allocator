cmake_minimum_required(VERSION 3.10)

project(YaBlockAllocator)

set(CMAKE_C_STANDARD 99)

find_package(Threads REQUIRED)
add_library(ya_block_allocator STATIC
    ya_block_allocator.c
    ya_block_allocator.h
)

add_executable(test_block_allocator
    test_block_allocator.c
)

target_link_libraries(test_block_allocator ya_block_allocator pthread)
