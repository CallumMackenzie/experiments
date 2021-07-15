cmake_minimum_required(VERSION 3.0.0)
project(math VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(../global)
add_library(math mathlib.cpp)
add_executable(tests_math tests.cpp)
add_test(math tests_math)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
