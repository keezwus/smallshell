cmake_minimum_required(VERSION 3.10)
project(shell LANGUAGES C)


set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


add_subdirectory(extcmd)
add_subdirectory(shell)

# enable_testing()
