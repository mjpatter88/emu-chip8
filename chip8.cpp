#include <iostream>
#include <string>
#include <stdio.h>
#include <SDL2/SDL.h>
#include "chip8.h"

// Debug output
#define DEBUG true
// Debug output - print memory contents
#define DEBUG_MEM true

int initRenderer(SDL_Window *window, SDL_Renderer **renderer);
int initSDL(SDL_Window **window);
void drawScreen(SDL_Renderer *renderer, Chip8 chip);



Chip8 chip;

int main(int argc, char **argv)
{
    // TODO
    // Set up the graphics system and the input system.
    SDL_Window *window;
    SDL_Renderer *renderer;
    if(initSDL(&window) != 0 || initRenderer(window, &renderer) != 0)
    {
        return 1;
    }
    drawScreen(renderer, chip);


    // Initialize the chip and load a rom
    std::string fileName;
    if(argc > 1)
    {
        fileName = argv[1];
    }
    else
    {
        fileName = "Test.ch8";
        std::cout << "Using \"" << fileName <<"\" as ROM" << std::endl;
    }
    chip.initialize();
    chip.loadRom(fileName);

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
    else
    {
        // Main emulation loop
        for(;;)
        {
            chip.emulateCycle();

            // Update the screen if necessary
            if(chip.getDrawFlag() == true)
            {
                // TODO
                // draw to the screen
                drawScreen(renderer, chip);
            }

            // Handle user input
        }
    }
    
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}

int initSDL(SDL_Window **window)
{
    SDL_Init(SDL_INIT_VIDEO);
    unsigned int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL;
    *window = SDL_CreateWindow("Chip 8 Display", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*10,
                                SCREEN_HEIGHT*10, flags);
    if(*window == NULL)
    {
        printf("Could not create SDL window: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        return 0;
    }
}

int initRenderer(SDL_Window *window, SDL_Renderer **renderer)
{
    *renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL)
    {
        printf("Could not create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }
    else
    {
        return 0;
    }
}

/*
 * Render the Chip 8's screen to an SDL window
 */
void drawScreen(SDL_Renderer *renderer, Chip8 chip)
{
    //TODO: implement drawing from the Chip8's screen array
    // to the SDL surface. Make each pixel 10x10.

    //Clear the screen to black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a white 10x10 rectangle
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect r;
    r.x=0;
    r.y=0;
    r.w=10;
    r.h=10;
    SDL_RenderFillRect(renderer, &r);
    SDL_RenderPresent(renderer);
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

    // TODO: implement the rest of this function
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






