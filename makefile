all : chip8

chip8 : chip8.o
	g++ chip8.o -lSDL2 -o chip8

chip8.o: chip8.cpp chip8.h
	g++ -c chip8.cpp -o chip8.o
