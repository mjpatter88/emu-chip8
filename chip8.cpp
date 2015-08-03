#include <iostream>
#include <stdio.h>
#include "chip8.h"

// Debug output
#define DEBUG true
// Debug output - print memory contents
#define DEBUG_MEM true

Chip8 chip;

int main(int argc, char **argv)
{
    // TODO
    // Set up the graphics system and the input system.
    // Use SDL for this?

    // Initialize the chip and load a rom
    chip.initialize();
    chip.loadRom("Test.ch8");

    // Debug loop. Press a key to execute one cycle and then
    // dump the memory/cpu state.
    if(DEBUG)
    {
        chip.dumpState();
        for(;;)
        {
            getchar();
            chip.emulateCycle();
            chip.dumpState();
        }
    }

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


/*
 * Initialize the CPU by setting the pc to 0x200 and reseting
 * other state variables.
 */
void Chip8::initialize()
{
    std::cout << "Initializing the Chip 8 emulator." << std::endl;
    pc = 0x200;     // Programs begin at location 0x200 (512)
    index_reg = 0;
    current_opcode = 0;
    sp = 0;

    //Blank screen to start
    for(int row=0; row<SCREEN_HEIGHT; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
           screen[row][col] = 0;
        }
    }

    // TODO: implement this function
}

void Chip8::loadRom(std::string file_name)
{
    std::cout << "Loading \"" << file_name << "\"..." << std::endl;
    // TODO: implement this function
}

/*
 * Emulate a single CPU cycle. Frequency = 60 Hz.
 */
void Chip8::emulateCycle()
{
    std::cout << "Emulating cycle..." << std::endl;

    // Fetch the opcode (two bytes, from "pc" location)
    current_opcode = memory[pc] << 8 | memory[pc+1];
    
    // Decode the opcode

    // Executre the opcode (and update "pc")

    // Update timers
}

bool Chip8::getDrawFlag()
{
    return draw_flag;
}

/*
 * A diagnostic function that prints current state info.
 */
void Chip8::dumpState()
{
    std::cout << "Dumping state..." << std::endl;
    std::cout << "\n";
    
    //TODO print cycle count
    printf("PC: 0x%X\t%d\n", pc, pc);
    printf("Opcode: 0x%X\n", current_opcode);
    for(int i=0; i<NUM_REG; i++)
    {
        printf("V%d: 0x%X\n", i, v_registers[i]);
    }

    //TODO: optionally dump memory to a file?
    if(DEBUG_MEM)
    {
        printf("********** Memory Contents **********\n");
        for(int i=0; i<MEM_SIZE; i+=2)
        {
            if(i%32 == 0)
            {
                if(i!=0)
                {
                    printf("\n");
                }
                printf("%03X (%04d): ", i, i);
            }
            printf("%02X%02X ", memory[i], memory[i+1]);
        }
        printf("\n");
    }

    printf("********** Screen Contents **********\n");
    for(int row=0; row<SCREEN_HEIGHT; row++)
    {
        for(int col=0; col<SCREEN_WIDTH; col++)
        {
            printf("%d", screen[row][col]);
        }
        printf("\n");
    }
}






