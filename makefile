xyz.ppm: x
	./x > xyz.ppm

x: main.cpp s2.h hw.h hl.h
	g++ -o x -g --std=c++17 main.cpp

