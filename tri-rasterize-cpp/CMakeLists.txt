cmake_minimum_required(VERSION 3.0.0)
project(triangle-rasterization VERSION 0.1.0)

include(CTest)
enable_testing()
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
include_directories(../global)

add_executable(triangle-rasterization main.cpp)
target_link_libraries(triangle-rasterization PRIVATE Threads::Threads)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
