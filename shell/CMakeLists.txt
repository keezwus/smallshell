add_executable(shell 
    src/shell.c 
    )


add_library(shellutils STATIC
    src/betterstr.c
    src/extarr.c
    src/sutils.c
    )

add_library(sigHandlers STATIC
    src/sigHandler.c
    )


target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(shellutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sigHandlers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(shell PRIVATE readline)
target_link_libraries(shell PRIVATE shellutils)
target_link_libraries(shell PRIVATE sigHandlers)


# add_subdirectory(test)