#include "Headers.h"
#include "Instructions.h"

Instructions::Instructions(Memorys* memorys, Registers* registers, Keyboard* keyboard, Screen* screen) {
    this->memorys = memorys;
    this->registers = registers;
    this->keyboard = keyboard;
    this->screen = screen;
}

Instructions::~Instructions() {

}

void Instructions::load(const char *buffer, size_t size) {
    assert(size + CHIP8_PROGRAM_LOAD_ADDRESS < CHIP8_MEMORY_SIZE);

    memcpy(&this->memorys->memory[CHIP8_PROGRAM_LOAD_ADDRESS], buffer, size);

    this->registers->programCounter = CHIP8_PROGRAM_LOAD_ADDRESS;
}

void Instructions::exec(uint16_t opcodes) {
    switch (opcodes) {
        case 0x00E0: { // CLS: The Display
            this->screen->clearScreen();
        }
        break;

        case 0x00EE: { // RET: Return from subrouting
            this->registers->programCounter = this->registers->stackPop();
        }
        break;

        default: {
            this->execExtend(opcodes);
        }
        break;
    }
}

void Instructions::execExtend(uint16_t opcodes) {
    uint16_t nnn = opcodes & 0x0FFF;
    uint8_t x = (opcodes >> 8) & 0x000F;
    uint8_t y = (opcodes >> 4) & 0x000F;
    uint8_t kk = opcodes & 0x00FF;
    uint8_t n = opcodes & 0x000F;

    switch (opcodes & 0xF000) {
        case 0x1000: { // JP addr, 1nnn Jump to location nnn
            this->registers->programCounter = nnn;
        }
        break;

        case 0x2000: { // CALL addr, Call subroutine at location nnn
            this->registers->stackPush(this->registers->programCounter);
            this->registers->programCounter = nnn;
        }  
        break;

        case 0x3000: { // SE Vx, byte - 3vkk Skip next instruction if Vx = kk
            if(this->registers->V[x] == kk) {
                this->registers->programCounter += 2;
            }
        }
        break;

        case 0x4000: { // SNE Vx, byte - 3vkk Skip next instruction if Vx != kk
            if(this->registers->V[x] != kk) {
                this->registers->programCounter += 2;
            }
        }
        break;

        case 0x5000: { // SE Vx, Vy - 5xy0 Skip next instruction if Vx = Vy
            if(this->registers->V[x] == this->registers->V[y]) {
                this->registers->programCounter += 2;
            }
        }
        break;

        case 0x6000: { // LD Vx, byte - 6xkk Set Vx = kk
            this->registers->V[x] = kk;
        }
        break;

        case 0x7000: { // ADD Vx, byte - 7xkk Set Vx = Vx + kk
            this->registers->V[x] += kk;
        }
        break;

        case 0x8000: {
            this->execExtendEight(opcodes);
        }
        break;
        
        case 0x9000: { // SNE Vx, Vy - 9xy0 Skip next instruction if Vx != Vy
            if(this->registers->V[x] != this->registers->V[y]) {
                this->registers->programCounter += 2;
            }
        }
        break;

        case 0xA000: { // LD I, addr - Annn Set I = nnn;
            this->registers->I = nnn;
        }
        break;

        case 0xB000: { // JP V0, addr - Bnnn Jump to location nnn + V0
            this->registers->programCounter = nnn + this->registers->V[0x00];
        }
        break;

        case 0xC000: { // RND Vx, byte - Cxkk Set Vx = random byte AND kk
            srand(clock());
            this->registers->V[x] = (rand() % 255) & kk;
        }
        break;

        case 0xD000: { // DRW Vx, Vy, nibble - Dxyn Display n-byte sprite starting at memory I at (Vx, Vy), set VF = collision
            this->registers->V[0x0F] = this->screen->drawSprite(this->registers->V[x], this->registers->V[y], &this->memorys->memory[this->registers->I], n);
        }
        break;

        case 0xE000: { 
            switch (opcodes & 0x00FF) {
                case 0x9E: { // SKP Vx - Ex9E Skip next instruction if key with value of Vx pressed
                    if(this->keyboard->keyboardIsDown(this->registers->V[x])) {
                        this->registers->programCounter += 2;
                    }
                }
                break;

                case 0xA1: { // SKNP Vx - ExA1 Skip next instruction if key with value of Vx not pressed
                    if(!this->keyboard->keyboardIsDown(this->registers->V[x])) {
                        this->registers->programCounter += 2;
                    }
                }
                break;
            }
        }
        break;

        case 0xF000: {
            this->execExtendF(opcodes);
        }
        break;
    }
}

void Instructions::execExtendEight(uint16_t opcodes) {
    uint8_t x = (opcodes >> 8) & 0x000F;
    uint8_t y = (opcodes >> 4) & 0x000F;
    uint8_t finalFourBits = opcodes & 0x000F;

    switch (finalFourBits) {
        case 0x00: { // LD Vx, Vy - 8xy0 Set Vx = Vy
            this->registers->V[x] = this->registers->V[y];
        }
        break;

        case 0x01: { // OR Vx, Vy - 8xy1 Set Vx = Vx OR Vy
            this->registers->V[x] = this->registers->V[x] | this->registers->V[y];
        }
        break;

        case 0x02: { // AND Vx, Vy - 8xy2 Set Vx AND Vy
            this->registers->V[x] = this->registers->V[x] & this->registers->V[y];
        }
        break;

        case 0x03: { // XOR Vx, Vy - 8xy3 Set Vx XOR Vy
            this->registers->V[x] = this->registers->V[x] ^ this->registers->V[y];
        }
        break;

        case 0x04: { // ADD Vx, Vy - 8xy4 Set Vx = Vx + Vy, set VF = carry
            this->registers->V[0x0F] = false;
            if(this->registers->V[x] + this->registers->V[y] > 0xFF) {
                this->registers->V[0x0F] = true;
            }
            this->registers->V[x] = this->registers->V[x] + this->registers->V[y];
        }
        break;

        case 0x05: { // SUB Vx, Vy - 8xy5 Set Vx = Vx - Vy, set VF = NOT borrow
            this->registers->V[0x0F] = false;
            if(this->registers->V[x] > this->registers->V[y]) {
                this->registers->V[0x0F] = true;
            }
            this->registers->V[x] = this->registers->V[x] - this->registers->V[y];
        }
        break;

        case 0x06: { // SHR Vx {, Vy} - 8xy6 Set Vx = Vx SHR 1
            this->registers->V[0x0F] = this->registers->V[x] & 0x01; // Note karen ndak pakai if (Paham dah 49:00)
            this->registers->V[x] = this->registers->V[x] / 2;
        }
        break;

        case 0x07: { // SUBN Vx, Vy - 8xy7 Set Vx = Vy - Vx, set VF = NOT borrow
            this->registers->V[0x0F] = this->registers->V[y] > this->registers->V[x];
            this->registers->V[x] = this->registers->V[y] - this->registers->V[x];
        }
        break;

        case 0x0E: { // SHL Vx {, Vy} - 8xyE Set Vx SHL 1
            this->registers->V[0x0F] = this->registers->V[x] & 0b10000000;
            this->registers->V[x] = this->registers->V[x] * 2;
        }
        break;
    }
}

void Instructions::execExtendF(uint16_t opcodes) {
    uint8_t x = (opcodes >> 8) & 0x000F;
    switch (opcodes & 0x00FF) {
        case 0x07: { // LD Vx, DT - Fx07 Set Vx = delay timer value
            this->registers->V[x] = this->registers->delayTimer;
        }
        break;

        case 0x0A: { // LD Vx, k - Fx0A Wait for a key press, stroe the value of the key in Vx
            this->registers->V[x] = this->waitKeyPress();
        }
        break;

        case 0x15: { // LD DT, Vx - Fx15 Set delay timer = Vx
            this->registers->delayTimer = this->registers->V[x];
        }
        break;

        case 0x18: { // LD ST, Vx - Fx18 Set sound timer = Vx
            this->registers->soundTimer = this->registers->V[x];
        }
        break;

        case 0x1E: { // ADD I, Vx - Fx1E Set I = I + Vx
            this->registers->I += this->registers->V[x];
        }
        break;

        case 0x29: { // LD F, Vx - Fx29 Set I = location of sprite for digit Vx
            this->registers->I = this->registers->V[x] * CHIP8_DEFAULT_SPRITE_HEIGHT;
        }
        break;

        case 0x33: { // LD B, Vx - Fx33 Store BCD representation of Vx in Memory locations I, I + 1 and I = 2
            this->memorys->setMemory(this->registers->I, this->registers->V[x] / 100);
            this->memorys->setMemory(this->registers->I + 1, this->registers->V[x] / 10 % 10);
            this->memorys->setMemory(this->registers->I + 2, this->registers->V[x] % 10);
        }
        break;

        case 0x55: {// LD [I], Vx - Fx55 Strore registers V0 through Vx in memory starting at location I
            for (int i = 0; i <= x; i++) {
                this->memorys->setMemory(this->registers->I + i, this->registers->V[i]);
            }
        }
        break;

        case 0x65: { // LD Vx, [I] - Fx65 Read registers V0 through Vx from memory starting at location I
            for (int i = 0; i <= x; i++) {
                this->registers->V[i] = this->memorys->getMemory(this->registers->I + i);
            }
        }
        break;
    }
}

char Instructions::waitKeyPress() {
    SDL_Event event;
    while(SDL_WaitEvent(&event)) {
        if(event.type != SDL_KEYDOWN) {
            continue;
        }
        char c = event.key.keysym.sym;
        char chip8key = this->keyboard->keyboardMap(c);
        if(chip8key != -1) {
            return chip8key;
        }
    }
    return -1;
}
