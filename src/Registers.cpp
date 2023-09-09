#include "Headers.h"
#include "Registers.h"

Registers::Registers(/* args */) {
    for (int i = 0; i < CHIP8_TOTAL_DATA_REGISTER; i++) {
        this->V[i] = 0;
    }
    this->I = 0;
    this->delayTimer = 0;
    this->soundTimer = 0;
    this->programCounter = 0;
    this->stackPointer = 0;
    for (int i = 0; i < CHIP8_TOTAL_STACK_DEPTH; i++) {
        this->stack[i] = 0;
    }
    
}

Registers::~Registers() {
}

void Registers::stackBounds() {
    assert(this->stackPointer < CHIP8_TOTAL_STACK_DEPTH);
}

void Registers::stackPush(uint16_t value) {
    this->stackBounds();
    this->stack[this->stackPointer] = value;
    this->stackPointer += 1;
}

uint16_t Registers::stackPop() {
    this->stackBounds();
    this->stackPointer -= 1;
    return this->stack[this->stackPointer];
}