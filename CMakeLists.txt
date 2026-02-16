cmake_minimum_required(VERSION 3.10.0)
project(search_engine VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(MYS_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(src)

add_subdirectory(tests)

