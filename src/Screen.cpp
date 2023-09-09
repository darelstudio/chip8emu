#include "Headers.h"
#include "Screen.h"

void Screen::screenBounds(int x, int y) {
    assert(x >= 0 && x < CHIP8_SCREEN_WIDTH && y >= 0 && y < CHIP8_SCREEN_HEIGHT);
}

Screen::Screen(/* args */) {
    this->clearScreen();
}

Screen::~Screen() {

}

void Screen::clearScreen() {
    memset(this->pixels, 0, sizeof(this->pixels));
}

void Screen::setScreen(int x, int y)
{
    this->screenBounds(x, y);
    this->pixels[x][y] = true;
}

bool Screen::screenIsSet(int x, int y) {
    this->screenBounds(x, y);
    return this->pixels[x][y];
}

bool Screen::drawSprite(int x, int y, uint8_t* sprite, int num) {
    bool pixelCollusion = false;

    for(int ly = 0; ly < num; ly++) {
        char c = sprite[ly];
        for(int lx = 0; lx < 8; lx++) {
            if((c & (0b10000000 >> lx)) == 0) {
                continue;
            }
            if(this->pixels[(lx + x) % CHIP8_SCREEN_WIDTH][(ly + y) % CHIP8_SCREEN_HEIGHT]) {
                pixelCollusion = true;
            }
            this->pixels[(lx + x) % CHIP8_SCREEN_WIDTH][(ly + y) % CHIP8_SCREEN_HEIGHT] ^= true;
        }
    }

    return pixelCollusion;
}
