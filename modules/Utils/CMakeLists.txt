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

target_sources(${PROJECT_NAME}
        PRIVATE ${SOURCES}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${INCLUDE_DIR}
        FILES ${HEADERS}
)

target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        $<BUILD_INTERFACE:${stb_SOURCE_DIR}>
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
)
