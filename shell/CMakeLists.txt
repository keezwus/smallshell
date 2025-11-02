add_executable(shell 
    src/shell.c 
    )


add_library(shellutils STATIC
    src/betterstr.c
    src/extarr.c
    )


target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(shellutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(shell PRIVATE readline)
target_link_libraries(shell PRIVATE shellutils)


# add_subdirectory(test)