#include "Emulator.h"

Emulator::Emulator(): cpu(&mmu) {
    cartridge.loadCartridge("Pokemon.gb");
}

Emulator::~Emulator() {
}