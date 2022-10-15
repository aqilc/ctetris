@echo off

x86_64-w64-mingw32-gcc -O3 -std="c18" -march="x86-64" -static -I".\deps\include" -L".\deps\lib" *.c ./util/*.c ./g/*.c -Wl,-Bstatic -lfreetype -lglew32 -lshell32 -lGdi32 -luser32 -lopengl32 -lpng -lpng16 -lglfw3

