cmake_minimum_required(VERSION 4.0.0)
project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        LANGUAGES C CXX
)
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_C_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/lib")
set(PLUGIN_DIR "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
set(PATH_VERSION "${PROJECT_BINARY_DIR}/include/CAE/Version.hpp")
set(TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/cmake/config/Version.hpp.in")

add_compile_definitions(PLUGINS_DIR="${PLUGIN_DIR}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJECT_FILES
        ${SOURCES}
        "${PROJECT_SOURCE_DIR}/include/*.hpp"
        "${PROJECT_SOURCE_DIR}/modules/*.cpp"
        "${PROJECT_SOURCE_DIR}/modules/*.hpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.cpp"
        "${PROJECT_SOURCE_DIR}/plugins/*.hpp"
)
set(FILES_TO_FORMAT ${PROJECT_FILES})

include(MakeVersion)
include(MakeDoc)
include(ClangTidy)
include(ClangFormat)
include(CopyAssets)
include(CompileOptions)

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
            GIT_TAG 1.0.3
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

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} cae-modules)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include" "${PROJECT_BINARY_DIR}/include" ${glm_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        nlohmann_json::nlohmann_json
        glm::glm
)

copy_directory_to_target(
        ${PROJECT_NAME}
        "${CMAKE_SOURCE_DIR}/assets"
        "assets"
)
