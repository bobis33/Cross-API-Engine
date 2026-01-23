cmake_minimum_required(VERSION 4.0.0)

project(cae
        VERSION 0.0.0
        DESCRIPTION "Cross-API graphics engine"
        HOMEPAGE_URL "https://bobis33.github.io/Cross-API-Engine/"
        LANGUAGES C CXX
)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/modules")

option(CAE_BUILD_DOC_HTML "Build html documentation" OFF)
option(CAE_BUILD_DOC_PDF "Build pdf documentation" OFF)
option(CAE_CLANG_FORMAT "Enable clang-format" OFF)
option(CAE_CLANG_TIDY "Enable clang-tidy static analysis" OFF)

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
endif ()

if (CAE_CLANG_TIDY)
    run_clang_tidy(${PROJECT_FILES})
endif ()

if (CAE_BUILD_DOC_HTML OR CAE_BUILD_DOC_PDF)
    find_package(Doxygen REQUIRED)
    cae_download_extra_css()
    if (CAE_BUILD_DOC_HTML)
        cae_add_doxygen_html(cae-doc-html ${PROJECT_FILES})
    elseif (CAE_BUILD_DOC_PDF)
        cae_add_doxygen_pdf(cae-doc-pdf ${PROJECT_FILES})
    endif ()
endif ()

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
