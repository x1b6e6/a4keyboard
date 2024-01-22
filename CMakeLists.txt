cmake_minimum_required(VERSION 3.20)
project(bpf-test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/include)

include(cmake/bpf.cmake)

add_subdirectory(bpf)
add_subdirectory(host)
