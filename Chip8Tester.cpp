#include "Chip8.h";
#include "Chip8Tester.h";
#include <iostream>;

void Chip8Tester::runTests() {
	bool allTestsPassed = true;
	std::cout << "Running tests...\n\n";
	
	allTestsPassed &= testClearScreen();
	allTestsPassed &= testSetRegisterToImmediateValue();
	allTestsPassed &= testSetI();


	if (allTestsPassed) {
		std::cout << "\n\n\nAll tests passed!";
	}
	else {
		std::cout << "\n\n\nTESTS FAILED!!!!";
	}
	return;
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
	if (success) {
		std::cout << "testSetI passed!" << std::endl;
	}
	else {
		std::cout << "*****testSetI failed!" << std::endl;
	}
	return success;
}