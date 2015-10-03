#ifndef TEST_H_
#define TEST_H_

#include "chip8.h";

class Chip8Tester : public Chip8
{
private:
	bool testClearScreen();
public:
	void runTests();

};

#endif