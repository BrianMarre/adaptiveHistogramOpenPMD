cmake_minimum_required(VERSION 3.13)

project(openpmd-example)

find_package(openPMD REQUIRED)

add_executable(${CMAKE_PROJECT_NAME} main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE openPMD::openPMD)
