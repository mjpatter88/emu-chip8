#ifndef TEST_H_
#define TEST_H_

#include "Chip8.h"

class Chip8Tester : public Chip8
{
private:
	bool testJumpToMachineCode();
	bool testClearScreen();
	bool testReturnFromSubroutine();
	bool testJumpLocation();
	bool testCallSubroutine();
	bool testSetRegisterToImmediateValue();
	bool testSetI();
	bool testDrawSprite();
	bool testAddImmediate();
	bool testSetDelayTimer();
	bool testSkipNotEqualImmediate_skip();
	bool testSkipNotEqualImmediate_noSkip();
	bool testSkipEqualImmediate_skip();
	bool testSkipEqualImmediate_noSkip();
	bool testSkipEqualRegister_skip();
	bool testSkipEqualRegister_noSkip();
	bool testAddRegister_noCarry();
	bool testAddRegister_Carry();
	bool testRandomAnd();
public:
	void runTests();

};

#endif