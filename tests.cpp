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

	return false;
}
