cmake_minimum_required(VERSION 3.0.0)
project(text-rendering-cpp VERSION 0.1.0)

include(CTest)
enable_testing()

link_directories(../global/GLFW)
include_directories(../global ../global/GLFW ../global/glad/include/KHR ../global/glad/include/glad)

add_executable(text-rendering-cpp main.cpp ../global/glad/glad.c)
target_link_libraries(text-rendering-cpp glfw3 glfw3_mt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
