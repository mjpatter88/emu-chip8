#include <string>

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
        unsigned char memory[4096];

        /*
         * https://en.wikipedia.org/wiki/CHIP-8#Registers
         * 16 registers, all 8-bits
         * Named V0-VF
         */
        unsigned char v_registers[16];

        /*
         * 16-bit address register
         */
        unsigned short index_reg;

        /*
         * 16-bit program counter
         */
        unsigned short pc;

        /*
         * https://en.wikipedia.org/wiki/CHIP-8#Graphics_and_sound
         * Display resolution of 64*32
         * Each pixel is either on (1) or off (0)
         */
        unsigned char screen[64 * 32];
        
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

    public:
        void initialize();
        void loadGame(std::string file_name);
        void emulateCycle();
        void setKeys();
        bool getDrawFlag();
};
