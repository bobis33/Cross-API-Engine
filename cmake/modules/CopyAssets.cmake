function(copy_directory_to_target TARGET SRC_DIR DST_SUBDIR)
    if (NOT TARGET ${TARGET})
        message(FATAL_ERROR "Target '${TARGET}' does not exist")
    endif()

    add_custom_command(
            TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${SRC_DIR}
            $<TARGET_FILE_DIR:${TARGET}>/${DST_SUBDIR}
    )
endfunction()
