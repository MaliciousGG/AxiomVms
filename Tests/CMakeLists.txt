include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    # GoogleTest v1.17.0, pinned to an immutable commit.
    GIT_TAG 52eb8108c5bdec04579160ae17225d66034bd723
)

FetchContent_MakeAvailable(googletest)

add_executable(
    AxiomVms.Tests
    Core/Cameras/CameraManagerTests.cpp
    Core/Cameras/CameraSessionTests.cpp

    Core/Json/JsonFileTests.cpp
    Core/Json/JsonParserTests.cpp
    Core/Json/JsonValueTests.cpp
)

target_link_libraries(
    AxiomVms.Tests
    PRIVATE
        AxiomVms.Core
        GTest::gtest_main
)

configure_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/Core/Json/JsonFileTestData/JsonFileContainsExpectedData_1.json"
        "${CMAKE_CURRENT_BINARY_DIR}/JsonFileContainsExpectedData_1.json"
        COPYONLY
)

include(GoogleTest)

gtest_discover_tests(AxiomVms.Tests)
