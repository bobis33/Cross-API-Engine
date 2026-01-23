cmake_minimum_required(VERSION 4.0.0)
project(cae-editor
        VERSION 0.0.0
        DESCRIPTION "CAE Editor"
        LANGUAGES C CXX
)

generate_version_header("${CMAKE_BINARY_DIR}/include/CAE/VersionEditor.hpp")

find_package(nlohmann_json QUIET CONFIG)
if (NOT nlohmann_json_FOUND)
    FetchContent_Declare(
            nlohmann-json
            GIT_REPOSITORY "https://github.com/nlohmann/json.git"
            GIT_TAG        "v3.12.0"
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(nlohmann-json)
endif ()

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

if (CAE_ENABLE_LTO)
    cae_enable_lto(${PROJECT_NAME})
endif ()

target_compile_definitions(${PROJECT_NAME} PRIVATE CAE_PLUGINS_DIR="${CMAKE_BINARY_DIR}/bin/lib")

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_BINARY_DIR}/include"
        ${glm_SOURCE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-compile-options
        cae-interfaces
        cae-utils
        cae-engine
        nlohmann_json::nlohmann_json
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

copy_directory_to_target(
        ${PROJECT_NAME}
        "${CMAKE_SOURCE_DIR}/assets"
        "assets"
)
