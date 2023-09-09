#include "Headers.h"
#include "Keyboard.h"

void Keyboard::keyboardBounds(int key) {
    assert(key >= 0 && key < CHIP8_TOTAL_KEYS);
}

Keyboard::Keyboard(/* args */) {
    for (int i = 0; i < CHIP8_TOTAL_KEYS; i++) {
        this->keyboard[i] = false;
    }
}

Keyboard::~Keyboard() {

}

int Keyboard::keyboardMap(char key) {
    for(int i = 0; i < CHIP8_TOTAL_KEYS; i++) {
        if(this->keyboardMapArr[i] == key) {
            return i;
        }
    }
    return -1;
}

void Keyboard::keyboardDown(int key) {
    this->keyboard[key] = true;
}

void Keyboard::keyboardUp(int key) {
    this->keyboard[key] = false;
}

bool Keyboard::keyboardIsDown(int key) {
    return this->keyboard[key];
}