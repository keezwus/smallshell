set(EXTCMD ls cat less xargs touch mkdir cp rm mv ln)

function(setExe name)
    add_executable(${name} src/${name}.cpp)
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endfunction()

foreach(exe IN LISTS EXTCMD)
    setExe(${exe})
endforeach()
