cmake_minimum_required(VERSION 3.10)
project(shell LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)


add_subdirectory(extcmd)
add_subdirectory(shell)

# enable_testing()
