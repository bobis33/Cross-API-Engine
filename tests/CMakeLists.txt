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

    target_link_libraries(${PROJECT_NAME} PRIVATE GTest::gtest_main cae-compile-options)
    target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
    include(GoogleTest)
    gtest_discover_tests(${PROJECT_NAME})
    set_property(TARGET ${PROJECT_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)

endif()
