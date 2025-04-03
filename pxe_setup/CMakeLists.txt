cmake_minimum_required(VERSION 3.10)
project(pxe_setup LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXXX_STANDARD_REQUIRED ON)

set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
set(INC_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${INC_DIR})

add_executable(pxe_setup ${SOURCES})


