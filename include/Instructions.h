#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

#include "Memorys.h"
#include "Registers.h"
#include "Keyboard.h"
#include "Screen.h"

class Instructions {
    private:
        Memorys* memorys;
        Registers* registers;
        Keyboard* keyboard;
        Screen* screen;

    public:
        Instructions(Memorys* memorys, Registers* registers, Keyboard* keyboard, Screen* screen);
        ~Instructions();

        void load(const char* buffer, size_t size);
        void exec(uint16_t opcodes);
        void execExtend(uint16_t opcodes);
        void execExtendEight(uint16_t opcodes);
        void execExtendF(uint16_t opcodes);
        char waitKeyPress();
};

#endif