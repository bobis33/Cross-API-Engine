project(cae-engine
        VERSION 0.0.0
        DESCRIPTION "CAE Engine library"
        LANGUAGES CXX
)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/Engine/*.hpp")

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
        cae-interfaces
        cae-utils
        glm::glm
)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        PRIVATE
        ${glm_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
)
