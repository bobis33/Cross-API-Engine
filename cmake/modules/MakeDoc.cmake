option(BUILD_DOC "Build documentation" OFF)

if(NOT BUILD_DOC)
    return()
endif()

# ----------------------------------------
# 1) Try system Doxygen
# ----------------------------------------
find_package(Doxygen QUIET)

# ----------------------------------------
# 2) Fetch Doxygen on Windows if missing
# ----------------------------------------
if(NOT DOXYGEN_FOUND AND WIN32)
    include(FetchContent)
    set(DOXYGEN_VERSION "1.16.1")
    set(DOXYGEN_URL "https://www.doxygen.nl/files/doxygen-${DOXYGEN_VERSION}.windows.x64.bin.zip")
    set(DOXYGEN_INSTALL_DIR "${CMAKE_BINARY_DIR}/_deps/doxygen")
    set(DOXYGEN_EXECUTABLE "${DOXYGEN_INSTALL_DIR}/doxygen.exe")

    if(NOT EXISTS "${DOXYGEN_EXECUTABLE}")
        message(STATUS "Downloading Doxygen ${DOXYGEN_VERSION}...")
        file(DOWNLOAD "${DOXYGEN_URL}" "${CMAKE_BINARY_DIR}/doxygen.zip" SHOW_PROGRESS)
        file(ARCHIVE_EXTRACT INPUT "${CMAKE_BINARY_DIR}/doxygen.zip" DESTINATION "${DOXYGEN_INSTALL_DIR}")
    endif()
    set(DOXYGEN_FOUND TRUE)
endif()

# ----------------------------------------
# 3) Fail if still not found
# ----------------------------------------
if(NOT DOXYGEN_FOUND)
    message(FATAL_ERROR "Doxygen not found and auto-fetch unavailable")
endif()

# ----------------------------------------
# 4) Configure Doxyfile
# ----------------------------------------
set(DOXYGEN_DIR "${CMAKE_SOURCE_DIR}/documentation/.doxygen")
set(DOXYFILE_OUT "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile")

configure_file("${DOXYGEN_DIR}/Doxyfile" "${DOXYFILE_OUT}" @ONLY)

# ----------------------------------------
# 5) Custom target 'doc' (Windows & Unix)
# ----------------------------------------
add_custom_target(doc ALL
        COMMAND "${DOXYGEN_EXECUTABLE}" "${DOXYFILE_OUT}"
        WORKING_DIRECTORY "${DOXYGEN_DIR}"
        COMMENT "Generating Doxygen documentation"
        VERBATIM
)

# Optional: copy refman.pdf if it exists
add_custom_command(TARGET doc POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${DOXYGEN_DIR}/latex/refman.pdf"
        "${CMAKE_SOURCE_DIR}/documentation/${PROJECT_NAME}.pdf"
        COMMENT "Copying PDF documentation if available"
        VERBATIM
)
