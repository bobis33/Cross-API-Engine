project(cae-opengl
        DESCRIPTION "CAE OpenGL Renderer Plugin"
        LANGUAGES CXX
)

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(WARNING "OpenGL not found, skipping ${PROJECT_NAME} build")
    return()
endif ()

include(FetchContent)
FetchContent_Declare(
        glad
        GIT_REPOSITORY "https://github.com/bobis33/glad"
        GIT_TAG ddd8a69bb693740db074d47616a8629637cc821a
        GIT_SHALLOW TRUE
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(glad)

if (APPLE)
    enable_language(OBJCXX)
elseif (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(EGL REQUIRED egl)
endif ()

file(GLOB_RECURSE SOURCES
        "${PROJECT_SOURCE_DIR}/src/*.cpp"
)

if (APPLE)
    file(GLOB_RECURSE SOURCES_MM "${PROJECT_SOURCE_DIR}/src/*.mm")
    set_source_files_properties(${SOURCES_MM} PROPERTIES LANGUAGE OBJCXX)
    list(APPEND SOURCES ${SOURCES_MM})
endif ()

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${OPENGL_INCLUDE_DIR}
        ${glm_SOURCE_DIR}
)
if (UNIX AND NOT APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE ${EGL_INCLUDE_DIRS})
endif ()

set(PLATFORM_LIBS cae-modules OpenGL::GL glad glm::glm)
if (UNIX AND NOT APPLE)
    list(APPEND PLATFORM_LIBS ${EGL_LIBRARIES})
elseif (APPLE)
    list(APPEND PLATFORM_LIBS "-framework AppKit")
endif ()

target_link_libraries(${PROJECT_NAME} PRIVATE ${PLATFORM_LIBS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)
