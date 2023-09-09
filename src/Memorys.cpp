#include "Headers.h"
#include "Memorys.h"

Memorys::Memorys(/* args */) {
    memset(this->memory, 0, sizeof(this->memory));

    memcpy(this->memory, characterSet, sizeof(characterSet));
}

Memorys::~Memorys() {
}

void Memorys::memoryBounds(int index) {
    assert(index >=0 && index < CHIP8_MEMORY_SIZE); //Pengecekan Error dari memory
}

void Memorys::setMemory(int index, uint8_t value) {
    this->memoryBounds(index);
    this->memory[index] = value;
}

unsigned char Memorys::getMemory(int index) {
    this->memoryBounds(index);
    return this->memory[index];
}

uint16_t Memorys::getShort(int index) {
    uint8_t byte1 = this->getMemory(index);
    uint8_t byte2 = this->getMemory(index + 1);
    return byte1 << 8 | byte2;
}
