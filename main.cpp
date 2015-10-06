#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include <iostream>
#include <string>
#include <stdio.h>
#include "Chip8.h"
#include "Chip8Tester.h"

#define TESTING false

int start(int argc, char* argv[]);
int initRenderer(SDL_Window *window, SDL_Renderer **renderer);
int initSDL(SDL_Window **window);
void drawScreen(SDL_Renderer *renderer, Chip8 chip);
void drawPixel(SDL_Renderer *renderer, int row, int col);
unsigned int cycleCallback(unsigned int interval, void *param);

Chip8 chip;

int main(int argc, char **argv)
{
	int retVal = 0;
	if (TESTING) {
		Chip8Tester chipTest;
		chipTest.runTests();
	}
	else {
		retVal = start(argc, argv);
	}

	if (DEBUG) {
		printf("\n\n\n\n\nPress any key to exit...");
		getchar();
	}
	return retVal;
}

/*
 * Start executing the chip8 emulator using SDL for input/graphics.
 */
int start(int argc, char* argv[]) {
    // Set up the graphics system and the input system (SDL).
    SDL_Window *window;
    SDL_Renderer *renderer;
    if(initSDL(&window) != 0 || initRenderer(window, &renderer) != 0) {
        return 1;
    }

    // Initialize the chip and load a rom
    std::string fileName;
    if(argc > 1) {
        fileName = argv[1];
    }
    else {
        fileName = "Test.ch8";
        std::cout << "Using \"" << fileName <<"\" as ROM" << std::endl;
    }
    chip.initialize();
    chip.loadRom(fileName);

    // Emulation and SDL event loop flags
    long count = 0;
    bool user_quit = false;

    // Set a new timer for the next cycle
    SDL_TimerID tid = SDL_AddTimer((1000/FREQ), cycleCallback, NULL);

    SDL_Event cur_event;
    // Handle SDL events
    while(!user_quit && SDL_WaitEvent(&cur_event)) {
        switch(cur_event.type) {
            case SDL_QUIT:
                user_quit = true;
                break;
            case SDL_USEREVENT:
                // Give input to the emulator
                // TODO: key presses to emulator
                if(count % 60 ==0) {
                    std::cout << "Cycle " << count << std::endl;
                }
                chip.emulateCycle();
                if(chip.getDrawFlag()) {
                    drawScreen(renderer, chip);
                }
                count++;
                break;
            default:
                //printf("Unhandled event\n");
                break;
        }
    } 
    
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

/*
 * This function gets called by the SDL_Timer 60 times per second. It is used to add events
 * to the SDL event system to make the cpu run at the proper frequency.
 */
unsigned int cycleCallback(unsigned int interval, void *param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    SDL_PushEvent(&event);
    return(interval);
}

/*
 * Initialize the necessary SDL systems and create the window.
 */
int initSDL(SDL_Window **window)
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER);
    unsigned int flags = 0; //It seems that we don't need any flags here.
    *window = SDL_CreateWindow("Chip 8 Display", SDL_WINDOWPOS_UNDEFINED,
                                SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH*10,
                                SCREEN_HEIGHT*10, flags);
    if(*window == NULL) {
        printf("Could not create SDL window: %s\n", SDL_GetError());
        return 1;
    }
    else {
        return 0;
    }
}

/*
 * Initialize the SDL renderer for the given window.
 */
int initRenderer(SDL_Window *window, SDL_Renderer **renderer)
{
    *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(*renderer == NULL) {
        printf("Could not create SDL renderer: %s\n", SDL_GetError());
        return 1;
    }
    else {
        return 0;
    }
}

/*
 * Render the Chip 8's screen to an SDL window. Each chip8 pixel will be
 * a 10x10 rectangle on the SDL window.
 */
void drawScreen(SDL_Renderer *renderer, Chip8 chip)
{
    //Clear the screen to black, then set drawing color back to white.
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Draw a white 10x10 rectangle for each chip8 pixel that is '1'
    for(int row=0; row<SCREEN_HEIGHT; row++) {
        for(int col=0; col<SCREEN_WIDTH; col++) {
            if(chip.screen[row][col] == 1) {
                drawPixel(renderer, row, col);
            }
        }
    }
    SDL_RenderPresent(renderer);
}

/*
 * Draw one "chip8 pixel" which will be 10x10 pixels in our SDL window.
 * "row" and "col" are in terms of the chip8 display, not the SDL window.
 *
 * Note: This function doesn't make the final "RenderPresent" call,
 * so if you want the pixel to show up on screen, you'll have to call
 * that yourself after you have drawn as many pixels as you want.
 */
void drawPixel(SDL_Renderer *renderer, int row, int col)
{
    //printf("ROW: %d, COL %d\n", row, col);
    SDL_Rect r;
    r.x=col*10;
    r.y=row*10;
    r.w=10;
    r.h=10;
    SDL_RenderFillRect(renderer, &r);
}
