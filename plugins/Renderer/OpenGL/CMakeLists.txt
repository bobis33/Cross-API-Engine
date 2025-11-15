project(cae-opengl
        DESCRIPTION "CAE OpenGL Renderer Plugin"
        LANGUAGES C CXX
)

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(WARNING "OpenGL not found, skipping ${PROJECT_NAME} build")
    return()
endif()

if (WIN32)
    message(STATUS "Building OpenGL plugin for Windows (WGL)")
elseif (APPLE)
    message(STATUS "Building OpenGL plugin for macOS (NSGL)")
    enable_language(OBJCXX)
else()
    message(STATUS "Building OpenGL plugin for Linux (EGL)")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(EGL REQUIRED egl)
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.c" "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")

if (APPLE)
    file(GLOB_RECURSE SOURCE_MM "${PROJECT_SOURCE_DIR}/src/*.mm")
    set_source_files_properties(${SOURCE_MM} PROPERTIES
            LANGUAGE OBJCXX
    )
    list(APPEND SOURCES ${SOURCE_MM})
endif()

add_library(${PROJECT_NAME} SHARED
        ${SOURCES}
        ${HEADERS}
)

if(UNIX AND NOT APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE
            "${PROJECT_SOURCE_DIR}/include"
            ${OPENGL_INCLUDE_DIR}
            ${EGL_INCLUDE_DIRS}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
            cae-modules
            OpenGL::GL
            ${EGL_LIBRARIES}
    )
elseif (APPLE)
    target_include_directories(${PROJECT_NAME} PRIVATE
            "${PROJECT_SOURCE_DIR}/include"
            ${OPENGL_INCLUDE_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
            cae-modules
            OpenGL::GL
            "-framework AppKit"
            "-framework OpenGL"
    )
else ()
    target_include_directories(${PROJECT_NAME} PRIVATE
            "${PROJECT_SOURCE_DIR}/include"
            ${OPENGL_INCLUDE_DIR}
    )
    target_link_libraries(${PROJECT_NAME} PRIVATE
            cae-modules
            OpenGL::GL
    )
endif ()

target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)