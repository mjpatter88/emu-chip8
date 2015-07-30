#include <iostream>
#include "chip8.h"

Chip8 chip;

int main(int argc, char **argv)
{
    // TODO
    // Set up the graphics system and the input system.
    // Use SDL for this?

    // Initialize the chip and load a rom
    chip.initialize();
    chip.loadGame("Test");

    // Main emulation loop
    for(;;)
    {
        chip.emulateCycle();

        // Update the screen if necessary
        if(chip.getDrawFlag() == true)
        {
            // TODO
            // draw to the screen
        }

        // Handle user input
    }
}


void Chip8::initialize()
{
    std::cout << "Initializing the Chip 8 emulator." << std::endl;
    // TODO: implement this function
}

void Chip8::loadGame(std::string file_name)
{
    std::cout << "Loading " << file_name << "." << std::endl;
    // TODO: implement this function
}

void Chip8:: emulateCycle()
{
    std::cout << "Emulating cycle..." << std::endl;
}

bool Chip8::getDrawFlag()
{
    return draw_flag;
}
