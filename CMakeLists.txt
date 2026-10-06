cmake_minimum_required(VERSION 3.25)

project(
    AxiomVms
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/Build")

include(CTest)

add_subdirectory(Source)

if(BUILD_TESTING)
    add_subdirectory(Tests)
endif()

