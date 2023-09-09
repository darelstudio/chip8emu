#ifndef KEYBOARD_H
#define KEYBOARD_H

class Keyboard {
    private:
        bool keyboard[CHIP8_TOTAL_KEYS];
        const char keyboardMapArr[CHIP8_TOTAL_KEYS] = {SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5, SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f};

    public:
        void keyboardBounds(int key);

        Keyboard(/* args */);
        ~Keyboard();

        int keyboardMap(char key);

        void keyboardDown(int key);
        void keyboardUp(int key);
        bool keyboardIsDown(int key);
};

#endif