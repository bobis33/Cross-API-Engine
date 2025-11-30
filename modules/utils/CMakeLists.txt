cmake_minimum_required(VERSION 4.0.0)

project(utils
        VERSION 0.0.0
        DESCRIPTION "Utils library"
        LANGUAGES CXX
)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/Utils/*.hpp")

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PRIVATE ${SOURCES}
        PUBLIC  FILE_SET HEADERS BASE_DIRS ${INCLUDE_DIR} FILES ${HEADERS}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        "${CMAKE_SOURCE_DIR}/third-party/stb"
)

target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        PREFIX ""
)