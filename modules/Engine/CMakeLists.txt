project(cae-engine
        VERSION 0.0.0
        DESCRIPTION "CAE Engine library"
        LANGUAGES CXX
)

set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCES "${SRC_DIR}/*.cpp")
file(GLOB_RECURSE HEADERS "${INCLUDE_DIR}/Engine/*.hpp")

add_library(${PROJECT_NAME} STATIC)

target_sources(${PROJECT_NAME}
        PRIVATE ${SOURCES}
        PUBLIC FILE_SET HEADERS
        BASE_DIRS ${INCLUDE_DIR}
        FILES ${HEADERS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-utils
        cae-compile-options
        glm::glm
)
target_include_directories(${PROJECT_NAME}
        PUBLIC
        $<BUILD_INTERFACE:${INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
        ${glm_SOURCE_DIR}
        PRIVATE
        "${CMAKE_SOURCE_DIR}/modules/Interfaces/include"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
        POSITION_INDEPENDENT_CODE ON
        CXX_EXTENSIONS OFF
)
