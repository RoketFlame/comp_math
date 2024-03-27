cmake_minimum_required(VERSION 3.9)
project(task1 LANGUAGES C)

add_executable(task1 main.c)
target_compile_options(task1 PRIVATE -Wall -Wextra -pedantic -O3)

find_package(OpenMP REQUIRED)
target_link_libraries(task1 PRIVATE OpenMP::OpenMP_C m)
