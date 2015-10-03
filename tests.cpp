#include "chip8.h";
#include "tests.h";
#include <iostream>;

void Chip8Tester::runTests() {
	bool allTestsPassed = true;
	std::cout << "Running tests...\n";
	
	allTestsPassed &= testClearScreen();

	if (allTestsPassed) {
		std::cout << "\n\n\nAll tests passed!";
	}
	else {
		std::cout << "\n\n\nTESTS FAILED!!!!";
	}
	return;
}

bool Chip8Tester::testClearScreen() {
	bool success = true;

	screen[5][5] = 1;
	screen[10][10] = 1;
	clearScreen();
    for(int row=0; row<SCREEN_HEIGHT; row++) {
        for(int col=0; col<SCREEN_WIDTH; col++) {
			if (screen[row][col] != 0) {
				success = false;
			}
        }
    }

	return success;
}
