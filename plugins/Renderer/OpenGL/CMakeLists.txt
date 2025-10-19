project(cae-opengl
        DESCRIPTION "CAE OpenGL Renderer Plugin"
        LANGUAGES C CXX
)

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message(WARNING "OpenGL not found, skipping ${PROJECT_NAME} build")
    return()
endif()

file(GLOB_RECURSE SOURCE "${PROJECT_SOURCE_DIR}/src/glad/glad.c" "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HEADERS "${PROJECT_SOURCE_DIR}/include/*.hpp")

add_library(${PROJECT_NAME} SHARED
        ${SOURCE}
        ${HEADERS}
)

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${PROJECT_SOURCE_DIR}/include/OPGL"
        "${PROJECT_SOURCE_DIR}/include/OPGL/glad"
        ${OPENGL_INCLUDE_DIR}
)
target_compile_options(${PROJECT_NAME} PRIVATE ${WARNING_FLAGS})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        OpenGL::GL
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
        RUNTIME_OUTPUT_DIRECTORY "${PLUGIN_DIR}"
)