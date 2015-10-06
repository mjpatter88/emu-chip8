#include "Chip8.h";
#include "Chip8Tester.h";
#include <iostream>;

void Chip8Tester::runTests() {
	bool allTestsPassed = true;
	std::cout << "Running tests...\n\n";
	
	allTestsPassed &= testJumpToMachineCode();
	allTestsPassed &= testClearScreen();
	allTestsPassed &= testSetRegisterToImmediateValue();
	allTestsPassed &= testSetI();
	allTestsPassed &= testAdd();
	allTestsPassed &= testDrawSprite();
	allTestsPassed &= testSetDelayTimer();

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

bool Chip8Tester::testAdd() {
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


