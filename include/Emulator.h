#ifndef EMULATOR_H
#define EMULATOR_H

#include "Memorys.h"
#include "Registers.h"
#include "Keyboard.h"
#include "Screen.h"
#include "Instructions.h"

class Emulator {
    private:
        bool emuRunning;

        Memorys memorys;
        Registers registers;
        Keyboard keyboard;
        Screen screen;
    public:
        Emulator(/* args */);
        ~Emulator();

        void run(const char* buff, long size);
};
#endif