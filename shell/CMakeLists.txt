add_executable(shell 
    src/shell.c 
    src/extarr.c
    )


target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(shell PRIVATE readline)

# add_subdirectory(test)