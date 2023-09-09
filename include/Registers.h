#ifndef REGISTERS_H
#define REGISTERS_H

class Registers {
    private:
        uint16_t stack[CHIP8_TOTAL_STACK_DEPTH];

    public:
        uint8_t V[CHIP8_TOTAL_DATA_REGISTER];
        uint16_t I;

        uint8_t delayTimer;
        uint8_t soundTimer;

        uint16_t programCounter;
        uint8_t stackPointer;

    public:
        Registers(/* args */);
        ~Registers();

        void stackBounds();
        void stackPush(uint16_t value);
        uint16_t stackPop();
};

#endif