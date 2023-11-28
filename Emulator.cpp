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

void Emulator::handleInterrupts() {
    u8 IF = mmu.readByte(0xFF0F);
    u8 IE = mmu.readByte(0xFFFF);
    cpu.pushStackWord(cpu.getPC());
    
    switch (IF & IE) {
        case 0x1: { // VBlank
            cpu.setPC(0x40);
            break;
        }
        case 0x2: { // LCD
            cpu.setPC(0x48);
            break;
        }
        case 0x4: { // Timer
            cpu.setPC(0x50);
            break;
        }
        case 0x8: { // Serial
            cpu.setPC(0x40);
            break;
        }
        case 0xF: { // Joypad
            cpu.setPC(0x60);
            break;
        }
    }
}
