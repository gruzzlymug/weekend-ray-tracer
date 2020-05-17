xyz.ppm: rt
	./rt > xyz.ppm

x: rt.cpp
	g++ -o rt -g --std=c++17 rt.cpp

