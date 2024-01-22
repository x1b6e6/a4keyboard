cmake_minimum_required(VERSION 3.20)
project(bpf-test VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_C_COMPILER_ID STREQUAL "Clang")
    message(FATAL_ERROR "Only Clang compiler is supported at now")
endif()

if (NOT CMAKE_GENERATOR STREQUAL "Ninja")
    message(FATAL_ERROR "Only Ninja generator is supported at now")
endif()

include_directories(${CMAKE_SOURCE_DIR}/include)

include(cmake/bpf.cmake)

add_subdirectory(bpf)
add_subdirectory(host)
