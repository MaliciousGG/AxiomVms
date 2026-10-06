add_executable(AxiomVms.Application)

target_sources(
    AxiomVms.Application
    PRIVATE
        Private/AxiomApplication.cpp

        # Main Program Entry
        AxiomVms.cpp
)

target_include_directories(
    AxiomVms.Application
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/Public
)

target_link_libraries(
    AxiomVms.Application
    PRIVATE
        AxiomVms.Core
)
