#ifndef TEST_H_
#define TEST_H_

#include "Chip8.h";

class Chip8Tester : public Chip8
{
private:
	bool testClearScreen();
	bool testSetRegisterToImmediateValue();
	bool testSetI();
	bool testDrawSprite();
public:
	void runTests();

};

#endif