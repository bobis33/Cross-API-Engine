include_guard(GLOBAL)

set(DOXYGEN_DIR "${CMAKE_SOURCE_DIR}/documentation/.doxygen")
set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

function(cae_download_extra_css)
    file(DOWNLOAD "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.4.1/doxygen-awesome.css"
            "${DOXYGEN_DIR}/css/doxygen-awesome.css")
    file(DOWNLOAD "https://raw.githubusercontent.com/jothepro/doxygen-awesome-css/v2.4.1/doxygen-awesome-sidebar-only.css"
            "${DOXYGEN_DIR}/css/doxygen-awesome-sidebar-only.css")
endfunction()

function(cae_add_doxygen_html TARGET FILES)
    configure_file("${DOXYGEN_DIR}/Doxyfile" ${DOXYFILE_OUT} @ONLY)
    doxygen_add_docs(${TARGET} ${FILES})
    add_custom_command(TARGET ${TARGET} POST_BUILD
            WORKING_DIRECTORY ${DOXYGEN_DIR}
            COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
            VERBATIM
    )
endfunction()

function(cae_add_doxygen_pdf TARGET FILES)
    configure_file("${DOXYGEN_DIR}/Doxyfile" ${DOXYFILE_OUT} @ONLY)
    doxygen_add_docs(${TARGET} ${FILES})
    add_custom_command(TARGET ${TARGET} POST_BUILD
            WORKING_DIRECTORY "${DOXYGEN_DIR}/latex"
            COMMAND make > /dev/null
            COMMAND ${CMAKE_COMMAND} -E copy "refman.pdf" "${CMAKE_SOURCE_DIR}/documentation/${PROJECT_NAME}.pdf"
            BYPRODUCTS "${CMAKE_SOURCE_DIR}/documentation/${PROJECT_NAME}.pdf"
            VERBATIM
    )
endfunction()
