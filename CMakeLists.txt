cmake_minimum_required(VERSION 4.0.0)

project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")

option(CAE_CLANG_FORMAT "Enable clang-format" OFF)

file(GLOB_RECURSE PROJECT_FILES
        ${SOURCES}
        "${PROJECT_SOURCE_DIR}/apps/*.cpp"
        "${PROJECT_SOURCE_DIR}/apps/*.hpp"
        "${PROJECT_SOURCE_DIR}/modules/*.cpp"
        "${PROJECT_SOURCE_DIR}/modules/*.hpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.cpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.hpp"
)

include(MakeVersion)
include(MakeDoc)
include(ClangTidy)
include(ClangFormat)
include(CopyAssets)
include(CompileOptions)

if (CAE_CLANG_FORMAT)
    format_project(${PROJECT_FILES})
endif()

find_package(nlohmann_json QUIET CONFIG)
if (NOT nlohmann_json_FOUND)
    include(FetchContent)
    FetchContent_Declare(
            nlohmann-json
            GIT_REPOSITORY "https://github.com/nlohmann/json.git"
            GIT_TAG        "v3.12.0"
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    FetchContent_MakeAvailable(nlohmann-json)
endif ()
find_package(glm QUIET CONFIG)
if (NOT glm_FOUND)
    FetchContent_Declare(
            glm
            GIT_REPOSITORY "https://github.com/g-truc/glm.git"
            GIT_TAG        1.0.3
            GIT_SHALLOW    TRUE
            GIT_PROGRESS   TRUE
    )
    set(GLM_BUILD_TESTS OFF CACHE BOOL "" FORCE)
    set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(glm)
endif ()

add_subdirectory(modules)
add_subdirectory(plugins)
add_subdirectory(tests)
add_subdirectory(apps)
