cmake_minimum_required(VERSION 3.0.0)
project(text-rendering-cpp VERSION 0.1.0)

include(CTest)
enable_testing()

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

if (WIN32)
	set(FREETYPE_INCLUDE_DIRS ../global/freetype/include)
	set(FREETYPE_LIBRARY freetype)
endif()

add_subdirectory(../global/glfw glfw)
find_package(Freetype REQUIRED)

link_directories(../global/glfw ../global/freetype)
include_directories(
	../global 
	../global/glfw 
	../global/glad/include/KHR 
	../global/glad/include/glad 
	../global/glfw/include/GLFW 
	../game-lin-alg
	${FREETYPE_INCLUDE_DIRS}
)

add_executable(text-rendering-cpp main.cpp ../global/glad/glad.c)
target_link_libraries(text-rendering-cpp glfw ${FREETYPE_LIBRARIES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
