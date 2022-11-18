# ctetris

> Includes a random, hopefully pretty optimized 2d graphics lib. Very simple, with fast operations taking less code and effort, vice versa for less performant operations.

## Building

1. Set up environment. I usually put "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build" in as path and just call `vcvars32` for my 32bit lib setup
2. Ensure libs are in place/on path, although most of them should be in the deps dir
3. run "run.bat"
4. Binary will be generated as `main.exe`, just bundle the `res` folder with it and it will run :D


### Random Ideologies for 2dgraphics.h

why would you hide implementation details from the user if they're not really going to change, let them do whatever they want, just note:
	- they **can** change, so don't bank *too* hard on them, or just don't upgrade your vers
		- i'll see if i can provide diffs for vulnerabilities and small bugfixes, so you don't have to upgrade every time
	- it's so simple, i think people could take it as a learning opportunity or maybe feel motivated to contribute

it has to be dead simple and easy af to use, i'm trying my best to make it that way and providing templates to start off on immediately
