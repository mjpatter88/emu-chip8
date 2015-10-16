#ifdef _WIN32
	#define _CRT_SECURE_NO_DEPRECATE
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif

#include <iostream>
#include "Chip8.h"


/*
 * Initialize the CPU by setting the pc to 0x200 and reseting
 * other state variables.
 */
void Chip8::initialize()
{
    // std::cout << "Initializing the Chip 8 emulator." << std::endl;
    pc = PC_START;     // Programs begin at location 0x200 (512)
    index_reg = 0;
    current_opcode = 0;
    sp = 0;

    //Blank screen to start
    clearScreen();

	//Initialize the rng. (Weak/predictable implementation is fine for this case.)
	srand(time(NULL));

    // TODO: implement the rest of this function
	// Set all memory to '0's?
}

/*
 * Copy the contents of the file into the ROM. The file can't contain more
 * than (MEM_SIZE - PC_START) bytes.
 */
void Chip8::loadRom(std::string file_name)
{
    // Open the file
    std::cout << "Loading \"" << file_name << "\"..." << std::endl;
    FILE* rom_file = fopen(file_name.c_str(), "rb");
    if(rom_file == NULL) {
        perror("Error opening the ROM file");
        return;
    }

    // Make sure it can fit in memory
    fseek(rom_file, 0, SEEK_END);
    int num_bytes = ftell(rom_file);
    if(num_bytes >= (MEM_SIZE - PC_START)) {
        printf("This file is too large to load into memory. ");
        printf("File size: %d. Max allowed: %d.", num_bytes, MEM_SIZE-PC_START);
        return;
    }

    // Copy the contents into memory byte-by-byte starting at PC_START
    rewind(rom_file);
    if(fread(&memory[PC_START], 1, num_bytes, rom_file) != num_bytes) {
        perror("Problem reading the ROM file");
        return;
    }

    fclose(rom_file);
}

/*
 * Emulate a single CPU cycle. Frequency = 60 Hz.
 */
void Chip8::emulateCycle()
{
    // Fetch the opcode (two bytes, from "pc" location)
    current_opcode = memory[pc] << 8 | memory[pc+1];
    
    //We don't need to draw unless 0x00E0 or 0xDXYN are executed.
    draw_flag = 0;
	v_registers[0xF] = 0;

    // Update timers
	if (delay_timer > 0) {
		delay_timer--;
	}
	if (sound_timer > 0) {
		sound_timer--;
	}

    // Decode and execute the opcode
    // "https://en.wikipedia.org/wiki/CHIP-8#Opcode_table"
    switch(current_opcode & 0xF000)
    {
        case 0x0000:
            switch(current_opcode & 0x00FF)
            {
                case 0x00E0:
				{
					// 00E0 "Clears the screen."
					clearScreen();
					draw_flag = 1;
					pc = pc + 2;
					break;
				}
                case 0x00EE:
				{
					// 00EE "Returns from a subroutine."
					// Set pc to the address at the top of the stack.
					// Subtract one from the stack pointer.
					pc = stack[sp];
					sp = sp - 1;
					break;
				}
                default:
				{
					// 0NNN "Calls RCA 1802 program at address NNN. Not necessary for most ROMs."
					pc = (current_opcode & 0x0FFF);
					break;
				}
            }
            break;
        case 0x1000:
		{
			// 1NNN "Jumps to address NNN."
			pc = (current_opcode & 0x0FFF);
			break;
		}
        case 0x2000:
		{
			// 2NNN "Calls subroutine at NNN."
			// The stack pointer is incremented by 1.
			// The address of the next instruction (current pc + 2) is put on the stack.
			// The pc value is set to NNN.
			sp = sp + 1;
			stack[sp] = pc + 2;
			pc = (current_opcode & 0x0FFF);
			break;
		}
        case 0x3000:
		{
			// 3XNN "Skips the next instruction if VX equals NN."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int immediateVale = (current_opcode & 0x00FF);
			if (v_registers[registerIndex] == immediateVale) {
				pc = pc + 4;
			}
			else {
				pc = pc + 2;
			}
			break;
		}
        case 0x4000:
		{
			// 4XNN "Skips the next instruction if VX doesn't equal NN."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int immediateVale = (current_opcode & 0x00FF);
			if (v_registers[registerIndex] != immediateVale) {
				pc = pc + 4;
			}
			else {
				pc = pc + 2;
			}
			break;
		}
        case 0x5000:
		{
			// 5XY0 "Skips the next instruction if VX equals VY."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int registerIndex2 = (current_opcode & 0x00F0) >> 4;
			if (v_registers[registerIndex] == v_registers[registerIndex2]) {
				pc = pc + 4;
			}
			else {
				pc = pc + 2;
			}
			break;
		}
        case 0x6000:
		{
            // 6XNN "Set VX to NN."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int immediateVale = (current_opcode & 0x00FF);
			v_registers[registerIndex] = immediateVale;
			pc = pc + 2;
            break;
		}
        case 0x7000:
		{
			// 7XNN "Adds NN to VX."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int immediateValue = (current_opcode & 0x00FF);
			v_registers[registerIndex] += immediateValue;
			pc = pc + 2;
			break;
		}
        case 0x8000:
		{
			switch (current_opcode & 0x000F)
			{
				case 0x0000:
				{
					// 8XY0 "Sets VX to the value of VY."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0001:
				{
					// 8XY1 "Sets VX to VX OR VY."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0002:
				{
					// 8XY2 "Sets VX to VX AND VY."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0003:
				{
					// 8XY3 ""Sets VX to VX XOR VY.
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0004:
				{
					// 8XY4 "Adds VY to VX. VF is set to 1 if there is a carry, 0 otherwise."
					int registerIndex = (current_opcode & 0x0F00) >> 8;
					int registerIndex2 = (current_opcode & 0x00F0) >> 4;
					int sum = v_registers[registerIndex] + v_registers[registerIndex2];
					if (sum > 255) {
						v_registers[0xF] = 1;
						v_registers[registerIndex] = (sum & 0xFF);
					}
					else {
						v_registers[0xF] = 0;
						v_registers[registerIndex] = sum;
					}
					pc = pc + 2;
					break;
				}
				case 0x0005:
				{
					// 8XY5 "VY is subtracted fro VX. VF is set to 0 if there is a borrow, 1 otherwise."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0006:
				{
					// 8XY6 "Shifts VX right by one. VF is set to the value of the LSB of VX before the shift."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x0007:
				{
					// 8XY7 "Sets VX to VY minux VX. VF is set to 0 if there is a borrow, 1 otherwise."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				case 0x000E:
				{
					// 8XYE "Shift VX left by one. VF is set to the value of the MSB of VX before the shift."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
				default:
				{
					std::cout << "Unknown instruction: " << current_opcode << std::endl;
					break;
				}
			}
			break;
		}
        case 0x9000:
		{
			// 9XY0 "Skips the next instruction if VX doesn't equal VY."
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			int registerIndex2 = (current_opcode & 0x00F0) >> 4;
			if (v_registers[registerIndex] != v_registers[registerIndex2]) {
				pc = pc + 4;
			}
			else {
				pc = pc + 2;
			}
			break;
		}
        case 0xA000:
		{
			// ANNN "Sets I to the address NNN."
			index_reg = (current_opcode & 0x0FFF);
			pc = pc + 2;
			break;
		}
        case 0xB000:
		{
			// BNNN "Jumps to the address NNN plus V0."
            pc = v_registers[0] + (current_opcode & 0x0FFF);
			break;
		}
        case 0xC000:
		{
			// CXNN "Sets VX to the result of a bitwise AND operation on a random number and NN."
			// Generate a random number from 0 to 255, AND it with NN, set result in V[X]
			int randNum = rand() % 0xFF;
			int immediateValue = (current_opcode & 0x00FF);
			int registerIndex = (current_opcode & 0x0F00) >> 8;
			randNum = randNum & immediateValue;
			v_registers[registerIndex] = randNum;
			pc = pc + 2;
			break;
		}
        case 0xD000:
		{
			// DXYN "Draw a sprite at position VX, VY with N bytes of sprite data starting at the address stored in I."
			int column = v_registers[(current_opcode & 0x0F00) >> 8];
			int row = v_registers[(current_opcode & 0x00F0) >> 4];
			int numRows = current_opcode & 0x000F;
			for (int i = 0; i < numRows; i++) {
				for (int j = 0; j < 8; j++) {
					// Each byte is a row, each bit is a pixel
					int bitValue = (memory[index_reg + i] >> (7-j)) & 0b00000001;
					if (bitValue == 1) {
						if (screen[row + i][column + j] == 1) {
							//Set VF register
							v_registers[0xF] = 1;
							screen[row + i][column + j] = 0;
						}
						else {
							screen[row + i][column + j] = 1;
						}
					}
				}
			}

			pc = pc + 2;
			draw_flag = 1;
			break;
		}
        case 0xE000:
		{
            switch(current_opcode & 0x00FF)
            {
                case 0x009E:
				{
					// EX9E "Skips the next instruction if the key stored in VX is pressed."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x00A1:
				{
					// EXA1 "Skips the next instruction if the key stored in VX isn't pressed."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                default:
				{
					std::cout << "Unknown instruction: " << current_opcode << std::endl;
					break;
				}
            }
            break;
		}
        case 0xF000:
		{
            switch(current_opcode & 0x0FF)
            {
                case 0x0007:
				{
					// FX07 "Sets VX to the value of the delay timer.
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x000A:
				{
					// FX0A "A key press is awaited and then stored in VX."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x0015:
				{
					// FX15 "Sets the delay timer to VX."
					int registerIndex = (current_opcode & 0x0F00) >> 8;
					delay_timer = v_registers[registerIndex];
					pc = pc + 2;
					break;
				}
                case 0x0018:
				{
					// FX18 "Sets the sound timer to VX."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x001E:
				{
					// FX1E "Adds VX to I."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x0029:
				{
					// FX29 "Sets I to the location of the sprite for the character in VX.
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x0033:
				{
					// FX33 "Store the binary-coded decimal equivalent of the value stored in register VX at addresses I, I+1, and I+2."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x0055:
				{
					// FX55 "Stores V0 to VX in memory starting at address I."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                case 0x0065:
				{
					// FX65 "Fills V0 to VX with values from memory starting at address I."
					// TODO
					Chip8::unsupportedOpcode(current_opcode, pc);
					break;
				}
                default:
				{
					std::cout << "Unknown instruction: " << current_opcode << std::endl;
					break;
				}
            }
            break;
		}
        default:
		{
			std::cout << "Unknown instruction: " << current_opcode << std::endl;
			break;
		}
    }

}

/*
 * Returns the chip8's draw_flag.
 */
bool Chip8::getDrawFlag()
{
    return draw_flag;
}

/*
 * Clears the chip8's screen.
 */
bool Chip8::clearScreen()
{
    for(int row=0; row<SCREEN_HEIGHT; row++) {
        for(int col=0; col<SCREEN_WIDTH; col++) {
            screen[row][col] = 0;
        }
    }
	return true;
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
    for(int i=0; i<NUM_REG; i++) {
        printf("V%d: 0x%X\n", i, v_registers[i]);
    }

    //TODO: optionally dump memory to a file?
    if(DEBUG_MEM) {
        printf("********** Memory Contents **********\n");
        for(int i=0; i<MEM_SIZE; i+=2) {
            if(i%32 == 0) {
                if(i!=0) {
                    printf("\n");
                }
                printf("%03X (%04d): ", i, i);
            }
            printf("%02X%02X ", memory[i], memory[i+1]);
        }
        printf("\n");
    }

    printf("********** Screen Contents **********\n");
    for(int row=0; row<SCREEN_HEIGHT; row++) {
        for(int col=0; col<SCREEN_WIDTH; col++) {
            printf("%d", screen[row][col]);
        }
        printf("\n");
    }
}


/*
 * Print a notification that this opcode isn't implemented.
 * Also display the PC value and the instruction.
 */
void Chip8::unsupportedOpcode(unsigned short opcode, unsigned short pc)
{
	printf("Opcode not yet supported! PC: %X. Instruction: %X.\n", pc, opcode);
}



