add_executable(shell 
    src/shell.c 
    )


add_library(shellutils STATIC
    src/sutils.c
    )

add_library(sigHandlers STATIC
    src/sigHandler.c
    )

add_library(betterarr STATIC 
    src/betterarr.c
    )

target_include_directories(shell PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(shellutils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(sigHandlers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(betterarr PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)


target_link_libraries(shell PRIVATE readline)
target_link_libraries(shell PRIVATE shellutils)
target_link_libraries(shell PRIVATE sigHandlers)
target_link_libraries(shell PRIVATE betterarr)


# add_subdirectory(test)