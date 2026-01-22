cmake_minimum_required(VERSION 4.0.0)
project(cae-editor
        VERSION 0.0.0
        DESCRIPTION "Cae Editor"
        LANGUAGES C CXX
)

generate_version_header("${CMAKE_BINARY_DIR}/include/CAE/VersionEditor.hpp")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        "${CMAKE_BINARY_DIR}/include"
        ${glm_SOURCE_DIR}
)
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
