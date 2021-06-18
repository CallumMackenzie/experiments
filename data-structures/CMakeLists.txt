cmake_minimum_required(VERSION 3.0.0)
project(data-structures VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(data-structures main.cpp linked-list.h doubly-linked-list.h memory-aid.h)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
