@echo off
setlocal EnableDelayedExpansion

rem Sets up the env so `cl` is on the path
rem cl >nul
rem echo !errorlevel!
rem IF errorlevel 1 ( goto setup ) ELSE ( goto compile )

rem :setup
rem vcvars32
rem goto compile

rem :compile
mkdir bin
set c=clcache /c /MD /EHsc /O2 /std:c17 /Id:\projects\libs\include\ /I"d:\Apps and Files\Apps\vcpkg\installed\x86-windows\include" /Fo"./bin/"
for /f "tokens=*" %%F in ('dir /b /a:-d "*.c"') do call set c=%%c%% "%%F"
for /f "tokens=*" %%F in ('dir /b /a:-d ".\util\*.c"') do call set c=%%c%% ".\util\%%F"
for /f "tokens=*" %%F in ('dir /b /a:-d ".\g\*.c"') do call set c=%%c%% ".\g\%%F"
echo running %c%
%c%
link freetype.lib glew32s.lib shell32.lib Gdi32.lib user32.lib opengl32.lib glfw3.lib ./bin/*.obj /nologo /LIBPATH:d:\projects\libs\bin\ /NODEFAULTLIB:libcmt.lib /OUT:main.exe
rem  /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS

rem gcc -O3 -Id:\projects\libs\include\ -I"d:\Apps and Files\Apps\vcpkg\installed\x86-windows\include" main.c util.c -lfreetype -lglew32s -lshell32 -lGdi32 -luser32 -lopengl32 -lglfw3 -Ld:\projects\libs\bin

rem /ENTRY:mainCRTStartup /SUBSYSTEM:WINDOWS

rem cloc . --exclude-dir=res,.ccls-cache,.vscode

cd bin
del *.obj
cd ..
rmdir bin
