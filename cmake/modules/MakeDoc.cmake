option(BUILD_DOC "Build documentation" OFF)

if (NOT BUILD_DOC)
    return()
endif()

find_package(Doxygen REQUIRED)

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
