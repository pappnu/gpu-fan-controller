cmake_minimum_required(VERSION 3.20.0)
project(gpu-fan-controller VERSION 0.1.0)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

find_package(X11 REQUIRED)
find_package(NVCtrl REQUIRED)

message(STATUS "X11_FOUND = ${X11_FOUND}")
message(STATUS "NVCtrl_FOUND = ${NVCtrl_FOUND}")

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# define targets and sources
add_subdirectory(src)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
