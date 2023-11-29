#include "Emulator.h"
#include <iostream>
#include <fstream>

Emulator::Emulator(const char *fileName): cartridge(fileName), mmu(&cartridge, fileName), cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cpu.dumpRegisters();
    run();
}

void Emulator::run() {
    graphics = new Graphics(&mmu, &cpu);
}

Emulator::~Emulator() {
}

void Emulator::loop() {
    std::fstream logfile;
    logfile.open("log.txt", std::ios::out);
    int cyclesPassed = 0;
    while (cyclesPassed < CYCLES_PER_FRAME) {
        u16 PC = cpu.getPC();
        logfile <<"PC 0x" << std::hex << PC;

        u8 opCode = cpu.getInstruction();
        int cycles = cpu.executeInstruction(opCode);

        // print opcode and cycles
        printf("PC: 0x%04X OPCODE: %02X CYCLES: %d\n", PC, opCode, cycles);
        logfile << " OPCODE: " << std::hex << (int)opCode << " CYCLES: " << cycles << "\n";

        cpu.updateTimer(cycles);
        cyclesPassed += cycles;
        while (std::cin.get() != '\n');
        // UPDATE GRAPHICS WITH CYCLES
        handleInterrupts();
    }
    // RENDER GRAPHICS
    logfile.close();
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

u8 Emulator::readMemory(u16 addr) {
    return mmu.readByte(addr);
}
