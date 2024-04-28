cmake_minimum_required(VERSION 3.0.0)
project(binary-search-group VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 20)

include(CTest)
enable_testing()

add_executable(binary-search-group
src/Image.cpp src/Volume.cpp src/Filter.cpp src/Projection.cpp src/Slice.cpp src/Quickselect.cpp
        src/checkOutput.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
