@echo off

rem Sets up the env so `cl` is on the path
rem cl >nul
rem IF errorlevel 1 ( goto setup ) ELSE ( goto compile )

rem :setup
rem start vcvars32
rem goto compile

rem :compile

set c=clcache /c /MD /EHsc /O2 /std:c17 /Id:\projects\libs\include\ /I"d:\Apps and Files\Apps\vcpkg\installed\x86-windows\include"
for /f "tokens=*" %%F in ('dir /b /a:-d "*.c"') do call set c=%%c%% "%%F"
for /f "tokens=*" %%F in ('dir /b /a:-d "D:\projects\c\ctetris\util\*.c"') do call set c=%%c%% ".\util\%%F"
echo running %c%
%c%
link freetype.lib glew32s.lib shell32.lib Gdi32.lib user32.lib opengl32.lib glfw3.lib *.obj /nologo /LIBPATH:d:\projects\libs\bin\ /NODEFAULTLIB:libcmt.lib /OUT:main.exe

rem gcc -O3 -Id:\projects\libs\include\ -I"d:\Apps and Files\Apps\vcpkg\installed\x86-windows\include" main.c util.c -lfreetype -lglew32s -lshell32 -lGdi32 -luser32 -lopengl32 -lglfw3 -Ld:\projects\libs\bin

rem /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS

rem cloc . --exclude-dir=res,.ccls-cache,.vscode

del *.obj
