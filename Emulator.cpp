#include "Emulator.h"

Emulator::Emulator(const char *fileName): cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cartridge.loadCartridge(fileName);
}

Emulator::~Emulator() {
}