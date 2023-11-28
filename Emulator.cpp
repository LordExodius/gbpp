#include "Emulator.h"

Emulator::Emulator(const char *fileName): cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cartridge.loadCartridge(fileName);
    run();
    
}

void Emulator::run() {
    // graphics = new Graphics(&cpu, &cartridge, &mmu);
}

Emulator::~Emulator() {
}

void Emulator::loop() {
    int cyclesPassed = 0;
    while (cyclesPassed < CYCLES_PER_FRAME) {
        int opCode = cpu.getInstruction();
        int cycles = cpu.executeInstruction(opCode);
        cpu.updateTimer(cycles);
        cyclesPassed += cycles;
        // UPDATE GRAPHICS WITH CYCLES
        // CHECK AND SERVICE INTERRUPTS
    }
    // RENDER GRAPHICS
}