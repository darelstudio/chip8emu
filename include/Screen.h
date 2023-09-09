#ifndef SCREEN_H
#define SCREEN_H

class Screen {
    private:
        bool pixels[CHIP8_SCREEN_WIDTH][CHIP8_SCREEN_HEIGHT];
        
    public:
        static void screenBounds(int x, int y);

        Screen(/* args */);
        ~Screen();

        void clearScreen();
        void setScreen(int x, int y);
        bool screenIsSet(int x, int y);

        bool drawSprite(int x, int y, uint8_t* sprite, int num);
};

#endif