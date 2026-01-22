project(cae-window-x11
        DESCRIPTION "CAE X11 Window Plugin"
        LANGUAGES C CXX
)

if (NOT LINUX)
    message(WARNING "${PROJECT_NAME} can only be build on Linux")
    return()
endif ()

find_package(X11)
if (NOT X11_FOUND)
    message(WARNING "X11 not found, skipping ${PROJECT_NAME} build")
    return()
endif()

file(GLOB_RECURSE SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE
        "${PROJECT_SOURCE_DIR}/include"
        ${X11_INCLUDE_DIR}
)
target_link_libraries(${PROJECT_NAME} PRIVATE
        cae-modules
        ${X11_LIBRARIES}
)
set_target_properties(${PROJECT_NAME} PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)