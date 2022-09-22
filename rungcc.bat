@echo off

gcc -O3 -I".\deps\include" *.c ./util/*.c -llibfreetype -llibglew32 -lshell32 -lGdi32 -luser32 -lopengl32 -llibglfw3 -L".\deps\lib"
