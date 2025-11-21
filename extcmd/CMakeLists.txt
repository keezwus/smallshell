#set(EXTCMD ls cat less xargs touch mkdir cp rm mv ln)
set(EXTCMD ls)

function(setExe name)
    add_executable(${name} src/${name}.c)
    target_include_directories(${name} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
endfunction()

foreach(exe IN LISTS EXTCMD)
    setExe(${exe})
endforeach()
