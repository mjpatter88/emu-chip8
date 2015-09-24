#include <string>

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define NUM_REG 16
#define MEM_SIZE 4096
#define PC_START 0x200
#define FREQ  60

class Chip8 {
    private:
        /*
         * http://en.wikipedia.org/wiki/CHIP-8#Opcode_table
         * 35 possible opcodes, all 2 bytes long
         */
        unsigned short current_opcode;

        /*
         * https://en.wikipedia.org/wiki/CHIP-8#Memory
         * 4kb memory
         */
        unsigned char memory[MEM_SIZE];

        /*
         * https://en.wikipedia.org/wiki/CHIP-8#Registers
         * 16 registers, all 8-bits
         * Named V0-VF
         */
        unsigned char v_registers[NUM_REG];

        /*
         * 16-bit address register, used to index memory.
         */
        unsigned short index_reg;

        /*
         * 16-bit program counter
         */
        unsigned short pc;

        
        /*
         * This timer runs at 60 hertz until it reaches 0.
         * It can be set and read.
         */
        unsigned char delay_timer;

        /*
         * This timer runs at 60 hertz until it reaches 0.
         * When its value is nonzero, a beeping sound is made.
         */
        unsigned char sound_timer;

        /*
         * The stack has 16 levels, each storing a value of the program counter
         */
        unsigned short stack[16];
        unsigned short sp;

        /*
         * HEX based keypad (0x00 -> 0x0F)
         */
        unsigned char keypad[16];

        /*
         * Flag to signal when the screen needs to be drawn
         */
        bool draw_flag;

        /*
         * Utility function to clear the chip8's screen.
         */
        bool clearScreen();

        /*
         * Utility function that gets called whenever an unsupported opcode is encountered.
         */
        void unsupportedOpcode(unsigned short opcode, unsigned short pc);

    public:
        void initialize();
        void loadRom(std::string file_name);
        void emulateCycle();
        void setKeys();
        bool getDrawFlag();
        void dumpState();

        /*
         * https://en.wikipedia.org/wiki/CHIP-8#Graphics_and_sound
         * Display resolution of 64*32
         * Each pixel is either on (1) or off (0)
         */
        unsigned char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
        //unsigned char screen[SCREEN_WIDTH * SCREEN_HEIGHT];
};
