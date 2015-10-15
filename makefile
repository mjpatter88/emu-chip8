all : main

main : Chip8.o Chip8Tester.o main.o
	g++ Chip8.o Chip8Tester.o main.o -lSDL2 -o main

Chip8.o: Chip8.cpp Chip8.h
	g++ -c Chip8.cpp -o Chip8.o

Chip8Tester.o: Chip8Tester.cpp Chip8Tester.h
	g++ -c Chip8Tester.cpp -o Chip8Tester.o

main.o: main.cpp 
	g++ -c main.cpp -o main.o

clean:
	rm -f *.o
	rm -f main
