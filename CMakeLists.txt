cmake_minimum_required(VERSION 3.22)
project(ctetris C)

set(CMAKE_C_STANDARD 17)

include_directories(. d:/projects/libs/include "d:/Apps and Files/Apps/vcpkg/installed/x86-windows/include")
link_directories(d:/projects/libs/bin)
link_libraries(freetype.lib glew32s.lib shell32.lib Gdi32.lib user32.lib opengl32.lib glfw3.lib)

add_executable(ctetris
        util/hash.c
        util/hashfunc.c
        glapi.c
        graphics.c
        main.c
        tetris.c
        util.c)
