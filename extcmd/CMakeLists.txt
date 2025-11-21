add_executable(ls
    src/ls.c
    )

add_executable(cat
    src/cat.c
    )

add_executable(less
    src/less.c
    )

add_executable(xargs
    src/xargs.c
    )

add_executable(touch
    src/touch.c
    )

add_executable(mkdir
    src/mkdir.c
    )

add_executable(cp
    src/cp.c
    )

add_executable(rm
    src/rm.c
    )

add_executable(mv
    src/mv.c
    )

add_executable(ln
    src/ln.c
    )

target_include_directories(ls PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cat PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(less PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(xargs PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(touch PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(mkdir PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(cp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(rm PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(mv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(ln PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)