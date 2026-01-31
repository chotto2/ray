cmake_minimum_required(VERSION 3.16)
project(ray C)

set(CMAKE_C_STANDARD 11)

add_executable(ray src/ray.c)
target_include_directories(ray PRIVATE src)
target_link_libraries(ray gmp m)
