project(cae-utils
        VERSION 0.0.0
        DESCRIPTION "CAE Utils library"
        LANGUAGES CXX
)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/Utils/*.hpp")

include(FetchContent)
FetchContent_Declare(
        stb
        GIT_REPOSITORY "https://github.com/nothings/stb.git"
        GIT_TAG        "f1c79c02822848a9bed4315b12c8c8f3761e1296"
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(stb)

add_library(${PROJECT_NAME} STATIC)

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_sources(${PROJECT_NAME}
        PRIVATE ${SOURCES}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${INCLUDE_DIR}
        FILES ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        $<BUILD_INTERFACE:${stb_SOURCE_DIR}>
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib"
)
