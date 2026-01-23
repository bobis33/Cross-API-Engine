option(CAE_BUILD_TESTS "Build tests" OFF)
if (CAE_BUILD_TESTS)

    project(cae-tests
            DESCRIPTION "CAE unit tests"
            LANGUAGES CXX
    )

    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY "https://github.com/google/googletest.git"
            GIT_TAG        "v1.17.0"
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(googletest)

    file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

    add_executable(${PROJECT_NAME} ${SOURCES})

    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main cae-modules)
    target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
    set_target_properties(${PROJECT_NAME} PROPERTIES
            CXX_STANDARD 23
            CXX_STANDARD_REQUIRED ON
            POSITION_INDEPENDENT_CODE ON
            CXX_EXTENSIONS OFF
            RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

endif()
