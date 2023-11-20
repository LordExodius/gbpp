#include "Emulator.h"

Emulator::Emulator(const char *fileName): cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cartridge.loadCartridge(fileName);
    run();
    
}

void Emulator::run() {
    graphics = new Graphics(&cpu, &cartridge);
    graphics->run();
}

Emulator::~Emulator() {
}