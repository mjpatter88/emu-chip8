# emu-chip8 #
## A Chip 8 emulator written in C++ ##
### Work-in-progress ###


### TODO ###
1. **DONE**: Input a filename to use as a ROM.
2. **DONE**: Read from the ROM and correctly initialize memory.
3. **DONE**: Keep count on cycles and only run at 60 mhz.
4. Implement each opcode
5. **DONE**: Draw screen using SDL
6. Handle input using SDL
7. Add sample ROMs and include a link.
8. Add the "fontset" functionality.





### Development steps ###
1. CPU header/class with memory, registers, etc.
2. CPU function stubs and a basic emulation loop.
3. Debugging support.
4. Tests, tests, tests.
5. ROM file support.
6. Cycle counting and throttling, etc.
7. Graphical Display support.
8. Input support.


To install SDL2 on Ubuntu: <br>
"sudo apt-get install libsdl2-dev"


### Links ###
- http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
- https://en.wikipedia.org/wiki/CHIP-8
- http://mattmik.com/chip8.html
- http://devernay.free.fr/hacks/chip8/chip8def.htm
- http://emulator101.com/chip-8-instruction-set/
