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
    
    Core/Json/JsonParserTests.cpp
    Core/Json/JsonValueTests.cpp
)

target_link_libraries(
    AxiomVms.Tests
    PRIVATE
        AxiomVms.Core
        GTest::gtest_main
)

include(GoogleTest)

gtest_discover_tests(AxiomVms.Tests)
