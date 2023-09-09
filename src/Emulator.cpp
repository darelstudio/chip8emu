#include "Headers.h"
#include "Emulator.h"

Emulator::Emulator(/* args */) {
    this->emuRunning = true;
}

Emulator::~Emulator() {

}

void Emulator::run(const char* buff, long size) {
    Instructions instructions(&this->memorys, &this->registers, &this->keyboard, &this->screen);

    if(SDL_Init(SDL_INIT_VIDEO) > 0) {
        std::cout << "SDL_Init HAS FAILED. SDL_ERROR: " << SDL_GetError() << std::endl;
    }

    instructions.load(buff, size);

    std::cout << "WINDOW OPEN" << std::endl;

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window* window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, CHIP8_SCREEN_WIDTH * WINDOW_MULTIPLIER,  CHIP8_SCREEN_HEIGHT * WINDOW_MULTIPLIER, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET);

    while(this->emuRunning) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    this->emuRunning = false;
                break;

                case SDL_KEYDOWN: {
                    char key = event.key.keysym.sym;
                    int vkey = keyboard.keyboardMap(key);
                    if(vkey != -1) {
                        keyboard.keyboardDown(vkey);
                    }
                }
                break;

                case SDL_KEYUP: {
                    char key = event.key.keysym.sym;
                    int vkey = keyboard.keyboardMap(key);
                    if(vkey != -1) {
                        keyboard.keyboardUp(vkey);
                    }
                }
                break;
            };
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);

        for(int x = 0; x < CHIP8_SCREEN_WIDTH; x++) {
            for(int y = 0; y < CHIP8_SCREEN_HEIGHT; y++) {
                if(this->screen.screenIsSet(x, y)) {
                    SDL_Rect r;
                    r.x = x * WINDOW_MULTIPLIER;
                    r.y = y * WINDOW_MULTIPLIER;
                    r.w = WINDOW_MULTIPLIER;
                    r.h = WINDOW_MULTIPLIER;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }

        SDL_RenderPresent(renderer);

        if(this->registers.delayTimer > 0) {
            Sleep(1);
            this->registers.delayTimer -= 1;
        }

        if(this->registers.soundTimer > 0) {
            Beep(15000, 10 * this->registers.soundTimer);
            this->registers.soundTimer = 0;
        }

        uint16_t opcodes = this->memorys.getShort(this->registers.programCounter);
        this->registers.programCounter += 2;
        instructions.exec(opcodes);
    }

    SDL_DestroyWindow(window);

    std::cout << "WINDOW CLOSE" << std::endl;
}
