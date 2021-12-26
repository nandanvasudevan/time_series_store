cmake_minimum_required(VERSION 3.2)

project(time_based_keyValue_store)
set(CMAKE_CXX_STANDARD 20)

include(./build/conan/conanbuildinfo.cmake)
include(./build/conan/conan_paths.cmake)
include(./build/conan/Findfmt.cmake)
include(./build/conan/Findspdlog.cmake)

conan_basic_setup()

set(LIBS lib)

option(TEST_ENABLE_BENCHMARKING "Enable benchmarking" OFF)

include_directories(./include)

add_subdirectory(src)