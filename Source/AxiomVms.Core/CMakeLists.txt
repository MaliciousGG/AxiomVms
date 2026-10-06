add_library(AxiomVms.Core STATIC)

find_package(PkgConfig REQUIRED)

pkg_check_modules(
    GSTREAMER
    REQUIRED
    IMPORTED_TARGET
    gstreamer-1.0
)

include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Core.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Cameras.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/CMake/Streaming.cmake)

target_include_directories(
    AxiomVms.Core
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(
    AxiomVms.Core
    PUBLIC
        PkgConfig::GSTREAMER
)

target_compile_features(
    AxiomVms.Core
    PUBLIC
        cxx_std_20
)
