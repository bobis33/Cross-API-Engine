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
set(PLUGIN_DIR "${CMAKE_BINARY_DIR}/plugins")
set(PATH_VERSION "${PROJECT_SOURCE_DIR}/include/CAE/Generated/Version.hpp")
set(TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/cmake/config/Version.hpp.in")

add_compile_definitions(PLUGINS_DIR="${PLUGIN_DIR}")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/include/*.hpp")
file(GLOB_RECURSE PROJECT_FILES
        ${SOURCES}
        ${HEADERS}
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

add_subdirectory(modules)
add_subdirectory(plugins)
add_subdirectory(tests)

include(FetchContent)
FetchContent_Declare(
        nlohmann-json
        GIT_REPOSITORY "https://github.com/nlohmann/json.git"
        GIT_TAG        "v3.12.0"
        GIT_SHALLOW    TRUE
        GIT_PROGRESS   TRUE
)
FetchContent_MakeAvailable(nlohmann-json)

add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} cae-modules)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        nlohmann_json::nlohmann_json
)
if (NOT WIN32 AND NOT APPLE)
    set(WARNING_FLAGS
            -Wall
            -Wextra
            -Wdeprecated-copy
            -Wmisleading-indentation
            -Wnull-dereference
            -Woverloaded-virtual
            -Wpedantic
            -Wshadow
            -Wsign-conversion
            -Wnon-virtual-dtor
            -Wunused
            -Wcast-align
            -Wno-padded
            -Wconversion
            -Wformat
            -Winit-self
            -Wmissing-include-dirs
            -Wold-style-cast
            -Wredundant-decls
            -Wswitch-default
            -Wundef
    )
else ()
    if (MSVC)
        set(WARNING_FLAGS /W3)
    else()
        set(WARNING_FLAGS -Wno-error)
    endif()
endif()
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS} /O2)
else ()
    target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS} -O3)
endif ()

copy_directory_to_target(
        cae
        "${CMAKE_SOURCE_DIR}/assets/icons"
        "assets/icons"
)
copy_directory_to_target(
        cae
        "${CMAKE_SOURCE_DIR}/assets/shaders"
        "assets/shaders"
)
