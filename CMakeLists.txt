cmake_minimum_required(VERSION 3.0)
project(block_queue)

add_compile_options(-Wall)

add_executable(performance performance.cpp)
target_link_libraries(performance pthread)

enable_testing()
add_subdirectory(tests)


