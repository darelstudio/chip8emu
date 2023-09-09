#ifndef HEADER_H
#define HEADER_H

#include <iostream>

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>

#include <stdint.h>

#include <SDL2/SDL.h>

#define WINDOW_MULTIPLIER 10
#define WINDOW_TITLE "Emulator"

#define CHIP8_SCREEN_WIDTH 64
#define CHIP8_SCREEN_HEIGHT 32

#define CHIP8_MEMORY_SIZE 4096

#define CHIP8_PROGRAM_LOAD_ADDRESS 0x200

#define CHIP8_DEFAULT_SPRITE_HEIGHT 5

#define CHIP8_TOTAL_DATA_REGISTER 16
#define CHIP8_TOTAL_STACK_DEPTH 16
#define CHIP8_TOTAL_KEYS 16
#define CHIP8_TOTAL_CHARACTER_SET 80

#endif