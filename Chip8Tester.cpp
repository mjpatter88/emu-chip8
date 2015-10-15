#include "Chip8.h"
#include "Chip8Tester.h"
#include <iostream>

void Chip8Tester::runTests() {
	bool allTestsPassed = true;
	std::cout << "Running tests...\n\n";
	
	allTestsPassed &= testJumpToMachineCode();
	allTestsPassed &= testClearScreen();
	allTestsPassed &= testReturnFromSubroutine();
	allTestsPassed &= testJumpLocation();
	allTestsPassed &= testCallSubroutine();
	allTestsPassed &= testSetRegisterToImmediateValue();
	allTestsPassed &= testSetI();
	allTestsPassed &= testAddImmediate();
	allTestsPassed &= testDrawSprite();
	allTestsPassed &= testSetDelayTimer();
	allTestsPassed &= testSkipNotEqualImmediate_skip();
	allTestsPassed &= testSkipNotEqualImmediate_noSkip();
	allTestsPassed &= testSkipEqualImmediate_skip();
	allTestsPassed &= testSkipEqualImmediate_noSkip();
	allTestsPassed &= testSkipEqualRegister_skip();
	allTestsPassed &= testSkipEqualRegister_noSkip();
	allTestsPassed &= testAddRegister_noCarry();
	allTestsPassed &= testAddRegister_Carry();
	allTestsPassed &= testRandomAnd();

	if (allTestsPassed) {
		std::cout << "\n\n\nAll tests passed!";
	}
	else {
		std::cout << "\n\n\nTESTS FAILED!!!!";
	}
	return;
}

bool Chip8Tester::testJumpToMachineCode() {
	initialize();
	bool success = true;

	// Set the next instruction to 0x0272to jump to 0x272.
	memory[PC_START] = 0x02;
	memory[PC_START + 1] = 0x72;
	emulateCycle();

	success = (pc == 0x272);
	if (success) {
		std::cout << "testJumpToMachineCode passed!" << std::endl;
	}
	else {
		std::cout << "*****testJumpToMachineCode failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testJumpLocation() {
	initialize();
	bool success = true;

	// Set the next instruction to 0x1213 jump to 0x213.
	memory[PC_START] = 0x12;
	memory[PC_START + 1] = 0x13;
	emulateCycle();

	success = (pc == 0x213);
	if (success) {
		std::cout << "testJumpLocation passed!" << std::endl;
	}
	else {
		std::cout << "*****testJumpLocation failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testCallSubroutine() {
	initialize();
	bool success = true;

	// Set the next instruction to 0x2214 jump to 0x214.
	memory[PC_START] = 0x22;
	memory[PC_START + 1] = 0x14;
	emulateCycle();

	// Make sure pc is correct and that the stack is handled correctly.
	success = (pc == 0x214);
	if (stack[1] != PC_START + 2) {
		success = false;
	}
	if (sp != 1) {
		success = false;
	}

	if (success) {
		std::cout << "testCallSubroutine passed!" << std::endl;
	}
	else {
		std::cout << "*****testCallSubroutine failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testReturnFromSubroutine() {
	initialize();
	bool success = true;

	// Initialize the stack and stack pointer
	sp = 1;
	stack[sp] = 0x224;

	// Set the next instruction to 0x00EE to return from a subroutine
	memory[PC_START] = 0x00;
	memory[PC_START + 1] = 0xEE;
	emulateCycle();

	// Make sure pc is correct and that the stack is handled correctly.
	success = (pc == 0x224);
	if (sp != 0) {
		success = false;
	}

	if (success) {
		std::cout << "testReturnFromSubroutine passed!" << std::endl;
	}
	else {
		std::cout << "*****testReturnFromSubroutine failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testClearScreen() {
	initialize();
	bool success = true;
	
	// Set a few pixels to "on"
	screen[5][5] = 1;
	screen[10][10] = 1;

	// Set the next instruction to 0x00E0 to clear the screen
	memory[PC_START] = 0x00;
	memory[PC_START + 1] = 0xE0;
	emulateCycle();

	// Make sure that the screen is clear.
    for(int row=0; row<SCREEN_HEIGHT; row++) {
        for(int col=0; col<SCREEN_WIDTH; col++) {
			if (screen[row][col] != 0) {
				success = false;
			}
        }
    }
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testClearScreen passed!" << std::endl;
	}
	else {
		std::cout << "*****testClearScreen failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSetRegisterToImmediateValue() {
	initialize();
	bool success = true;

	// Set the next instruction to 0x6511 to set V5 to 0x11;
	memory[PC_START] = 0x65;
	memory[PC_START + 1] = 0x11;
	emulateCycle();

	success = (v_registers[5] == 0x11);
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testSetRegisterToImmediateValue passed!" << std::endl;
	}
	else {
		std::cout << "*****testSetRegisterToImmediateValue failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSetI() {
	initialize();
	bool success = true;

	// Set the next instruction to 0xA513 to set I to 0x513
	memory[PC_START] = 0xA5;
	memory[PC_START + 1] = 0x13;
	emulateCycle();

	success = (index_reg == 0x513);
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testSetI passed!" << std::endl;
	}
	else {
		std::cout << "*****testSetI failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testDrawSprite() {
	initialize();
	bool success = true;

	// In order to draw a sprite, we need a sprite in memory at address I
	index_reg = 0x0;
	memory[0] = 0b01100000;
	memory[1] = 0b10010000;
	memory[2] = 0b10010000;			// Draw a "0"
	memory[3] = 0b10010000;
	memory[4] = 0b01100000;

	// We also need the X-coordinate in V[0] and the Y-coordinate in V[1]
	v_registers[0] = 10;
	v_registers[1] = 5;

	// Set the next instruction to 0xD015 to draw the sprite. (5 rows, using V[0] and v[1])
	memory[PC_START] = 0xD0;
	memory[PC_START + 1] = 0x15;
	emulateCycle();

	// Verify that the drawFlag is set and the correct screen pixels are on.
	if (getDrawFlag() == false) {
		success = false;
	}
	if ((screen[5][11] != 1) || (screen[5][12] != 1)
		|| (screen[6][10] != 1) || (screen[6][13] != 1)
		|| (screen[7][10] != 1) || (screen[7][13] != 1)
		|| (screen[8][10] != 1) || (screen[8][13] != 1)
		|| (screen[9][11] != 1) || (screen[9][12] != 1))
	{
		success = false;
	}
	if (pc != PC_START + 2) {
		success = false;
	}

	if (success) {
		std::cout << "testDrawSprite passed!" << std::endl;
	}
	else {
		std::cout << "*****testDrawSprite failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testAddImmediate() {
	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[5] = 12;

	// Set the next instruction to 0x750F to add 15 to V[5]
	memory[PC_START] = 0x75;
	memory[PC_START + 1] = 0x0F;
	emulateCycle();

	success = (v_registers[5] == 27);
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testAdd passed!" << std::endl;
	}
	else {
		std::cout << "*****testAdd failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSetDelayTimer() {
	initialize();
	bool success = true;

	// Set the next instruction to 0xF615 to set the delay timer to the value of V[6]
	v_registers[6] = 32;
	memory[PC_START] = 0xF6;
	memory[PC_START + 1] = 0x15;
	emulateCycle();

	success = (delay_timer == 32);
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testSetDelayTimer passed!" << std::endl;
	}
	else {
		std::cout << "*****testSetDelayTimer failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSkipNotEqualImmediate_skip() {

	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[1] = 5;

	// Set the next instruction to 0x4107 to skip since V[1] != 7
	memory[PC_START] = 0x41;
	memory[PC_START + 1] = 0x07;
	emulateCycle();

	success = (pc == PC_START + 4);
	if (success) {
		std::cout << "testSkipNotEqualImmediate_skip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipNotEqualImmediate_skip failed!" << std::endl;
	}
	return success;
}


bool Chip8Tester::testSkipNotEqualImmediate_noSkip() {

	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[1] = 5;

	// Set the next instruction to 0x4105 to not skip since V[1] == 5
	memory[PC_START] = 0x41;
	memory[PC_START + 1] = 0x05;
	emulateCycle();

	success = (pc == PC_START + 2);
	if (success) {
		std::cout << "testSkipNotEqualImmediate_noSkip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipNotEqualImmediate_noSkip failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSkipEqualImmediate_skip() {

	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[1] = 7;

	// Set the next instruction to 0x3107 to skip since V[1] == 7
	memory[PC_START] = 0x31;
	memory[PC_START + 1] = 0x07;
	emulateCycle();

	success = (pc == PC_START + 4);
	if (success) {
		std::cout << "testSkipEqualImmediate_skip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipEqualImmediate_skip failed!" << std::endl;
	}
	return success;
}


bool Chip8Tester::testSkipEqualImmediate_noSkip() {

	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[1] = 5;

	// Set the next instruction to 0x3107 to not skip since V[1] != 7
	memory[PC_START] = 0x31;
	memory[PC_START + 1] = 0x07;
	emulateCycle();

	success = (pc == PC_START + 2);
	if (success) {
		std::cout << "testSkipEqualImmediate_noSkip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipEqualImmediate_noSkip failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testSkipEqualRegister_skip() {

	initialize();
	bool success = true;

	// Set the initial value of the registers
	v_registers[1] = 7;
	v_registers[2] = 7;

	// Set the next instruction to 0x5120 to skip since V[1] == V[2]
	memory[PC_START] = 0x51;
	memory[PC_START + 1] = 0x20;
	emulateCycle();

	success = (pc == PC_START + 4);
	if (success) {
		std::cout << "testSkipEqualRegister_skip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipEqualRegister_skip failed!" << std::endl;
	}
	return success;
}


bool Chip8Tester::testSkipEqualRegister_noSkip() {

	initialize();
	bool success = true;

	// Set the initial value of the registers
	v_registers[1] = 5;
	v_registers[2] = 7;

	// Set the next instruction to 0x5120 to not skip since V[1] != V[2]
	memory[PC_START] = 0x51;
	memory[PC_START + 1] = 0x20;
	emulateCycle();

	success = (pc == PC_START + 2);
	if (success) {
		std::cout << "testSkipEqualRegister_noSkip passed!" << std::endl;
	}
	else {
		std::cout << "*****testSkipEqualRegister_noSkip failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testAddRegister_noCarry() {
	initialize();
	bool success = true;

	// Set the initial value of the registers
	v_registers[5] = 12;
	v_registers[6] = 13;

	// Set the next instruction to 0x8564 to add V[6] (13) to V[5]
	memory[PC_START] = 0x85;
	memory[PC_START + 1] = 0x64;
	emulateCycle();

	success = (v_registers[5] == 25);
	if (pc != PC_START + 2) {
		success = false;
	}
	if (v_registers[0xF] != 0) {
		success = false;
	}
	if (success) {
		std::cout << "testAddRegister_noCarry passed!" << std::endl;
	}
	else {
		std::cout << "*****testAddRegister_noCarry failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testAddRegister_Carry() {
	initialize();
	bool success = true;

	// Set the initial value of the register
	v_registers[5] = 12;
	v_registers[6] = 250;

	// Set the next instruction to 0x8564 to add V[6] (250) to V[5]
	memory[PC_START] = 0x85;
	memory[PC_START + 1] = 0x64;
	emulateCycle();

	success = (v_registers[5] == ((12+250)&0xFF));
	if (pc != PC_START + 2) {
		success = false;
	}
	if (v_registers[0xF] != 1) {
		success = false;
	}
	if (success) {
		std::cout << "testAddRegister_Carry passed!" << std::endl;
	}
	else {
		std::cout << "*****testAddRegister_Carry failed!" << std::endl;
	}
	return success;
}

bool Chip8Tester::testRandomAnd() {
	initialize();
	bool success = true;

	// Set the next instruction to 0xC555 to and a random num with 0x55 and store the result in V[5]
	memory[PC_START] = 0xC5;
	memory[PC_START + 1] = 0x55;
	emulateCycle();

	// Can't really test the randomness, so just make sure it moves to the next instruction for now.
	if (pc != PC_START + 2) {
		success = false;
	}
	if (success) {
		std::cout << "testRandomAnd passed!" << std::endl;
	}
	else {
		std::cout << "*****testRandomAnd failed!" << std::endl;
	}
	return success;
}
