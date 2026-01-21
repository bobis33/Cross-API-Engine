option(CAE_BUILD_DOC "Build documentation" OFF)

if (NOT CAE_BUILD_DOC)
    return()
endif()

find_package(Doxygen REQUIRED)

file(DOWNLOAD https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.4.1/doxygen-awesome.css
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen-awesome.css)
file(DOWNLOAD https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.4.1/doxygen-awesome-sidebar-only.css
        ${CMAKE_CURRENT_BINARY_DIR}/doxygen-awesome-sidebar-only.css)

set(DOXYGEN_DIR "${CMAKE_SOURCE_DIR}/documentation/.doxygen")
set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")
configure_file("${DOXYGEN_DIR}/Doxyfile" ${DOXYFILE_OUT} @ONLY)
doxygen_add_docs(doxygen ${PROJECT_FILES})
add_custom_command(TARGET doxygen POST_BUILD
        WORKING_DIRECTORY ${DOXYGEN_DIR}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
        VERBATIM
)
add_custom_command(TARGET doxygen POST_BUILD
        WORKING_DIRECTORY "${DOXYGEN_DIR}/latex"
        COMMAND make > /dev/null
        COMMAND ${CMAKE_COMMAND} -E copy "refman.pdf" "${CMAKE_SOURCE_DIR}/documentation/${PROJECT_NAME}.pdf"
        BYPRODUCTS "${CMAKE_SOURCE_DIR}/documentation/${PROJECT_NAME}.pdf"
        VERBATIM
)
