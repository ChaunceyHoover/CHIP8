#ifndef HEADER_CHIP8_HPP
#define HEADER_CHIP8_HPP

// Standard headers
#include <fstream>
#include <iostream>
#include <string>

class Chip8 {
public:
	// All initialization is done here vs an init method
	Chip8();
public:
	// Emulation methods
	void cycle();
	bool load_rom(std::string input);
public:
	// Loaded ROM name for purpose of setting window title
	bool draw_flag;
	std::string loaded_rom_name;

	// CHIP8 emulator has HEX-based input keyboard, so only 16 key states are needed
	unsigned char key[16];

	// CHIP8 has a 64 x 32, monochromous display. Thus, only two value needed to store state:
	// on (white) & off (black)
	unsigned char gfx[64 * 32];
private:
	// CHIP8 opcodes are two bytes, so an unsigned short fits the needs
	unsigned short opcode;

	// CHIP8 has 4KB of RAM
	unsigned char memory[4096];

	// CHIP8 has 15 8-bit general purpose registers (V[0-E]), and the 16th is a 'carry flag'.
	unsigned char V[16];

	// Index register and program counter can have a value from 0x000 to 0xFFF
	unsigned short I;
	unsigned short pc;

	// Because the CHIP8 has no interrupt or hardward registers, it has internal timers that count down at 60Hz.
	// When set above 0, they count down. The system buzzer sounds when `sound_timer` is set to 0.
	unsigned char delay_timer;
	unsigned char sound_timer;

	// Although the CHIP8 itself does not have a stack system, it _can_ jump to different instructions and subroutines,
	// so a stack and stack pointer is going to have to be implemented.
	unsigned short stack[16];
	unsigned short sp;

	/*
	Memory quick guide:
	0x000 - 0x1FF (0   -  511): CHIP8 Interpretter (Contains font set)
	0x000 - 0x050 (0   -   80): Built in 4x5 pixel font set (0-F)
	0x200 - 0xFFF (512 - 4095): Program ROM and working RAM
	*/
};

#endif