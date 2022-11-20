# ctetris

> Includes a random, hopefully pretty optimized 2d graphics lib. Very simple, with fast operations taking less code and effort, vice versa for less performant operations. I would love PRs working on optimizing this or adding non-invasive features :D

## Building

### Windows (my build env)
> Currently only builds on MSVC :'(. Please PR a way to build this with MinGW.

1. Set up environment. I usually put "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build" in as path and just call `vcvars32` for my 32bit lib setup
2. Ensure libs are in place/on path, although most of them should be in the deps dir
3. Either download [clcache](https://github.com/frerich/clcache)(what I use to speed up builds) or replace `clcache` in run.bat with `cl`
	- clcache can be installed with like one command `pip install git+https://github.com/frerich/clcache.git` so I'd recommend.
4. Run "run.bat"
5. Binary will be generated as `main.exe`, just bundle the `res` folder with it and it will run :D

### Linux (should be painless too though)

1. Install `gcc`, `pkg-config`, `freetype2`, `glfw3` and `glew`
2. run this command in bash: `gcc -o ctetris.exe $(pkg-config --static --cflags freetype2 glfw3 glew) -I./deps/include *.c ./util/*.c ./g/*.c -static $(pkg-config --static --libs freetype2 glfw3 glew) -lopengl32 -lstdc++`

### This won't work on Macs soon anyways so why even try to bulid on Mac lol

## Licensing

I know no sane person is going to try to use this in production, but I put a Commons Clause license on it for commercial users anyways. I'm going to college soon and I need as much money as I can get :(

Please Email me(aqilcm@gmail.com) about it and I'll try to do my best to respond. Feel free to spam it as long as I don't.


