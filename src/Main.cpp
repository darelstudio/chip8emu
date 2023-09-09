#include "Headers.h"

#include "Emulator.h"

int main(int argc, char** argv) {

    if(argc < 2) {
        std::cout << "Need a file" << std::endl;
        return -1;
    }

    const char* filename = argv[1];
    std::cout << "Filename: " << filename << std::endl;

    FILE* f = fopen(filename, "rb");
    if(!f) {
        std::cout << "Failed to open file" << std::endl;
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    char buff[size];
    int res = fread(buff, size, 1, f);
    if(res != 1) {
        std::cout << "Failed to read file" << std::endl;
        return -1;
    }

    Emulator emu;
    emu.run(buff, size);

    return 0;
}
