#include "Emulator.h"
#include <iostream>
#include <fstream>

Emulator::Emulator(const char *fileName): cartridge(fileName), mmu(&cartridge, fileName), cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cpu.dumpRegisters();
    window.create(sf::VideoMode(160, 144), "Gameboy Emulator");
    run();
}

void Emulator::run() {
    graphics = new Graphics(&mmu, &cpu, &window);
    while (graphics->isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }
        loop();
    }
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
        if (PC == 0x0216)
            while (std::cin.get() != '\n');
        u8 opCode = cpu.getInstruction();
        int cycles = cpu.executeInstruction(opCode);

        // print opcode and cycles
        // printf("\nPC: 0x%04X OPCODE: %02X CYCLES: %d\n", PC, opCode, cycles);
        logfile << " OPCODE: " << std::hex << (int)opCode << " CYCLES: " << cycles << "\n";

        cpu.updateTimer(cycles);
        // cpu.dumpRegisters();
        cyclesPassed += cycles;
        graphics->updateArray(cycles);
        handleInterrupts();
    }
    graphics->updateDisplay();
    logfile.close();
}

void Emulator::handleInterrupts() {
    if (!cpu.getIME()) {
        // printf("IME is disabled\n");
        return;
    }

    cpu.setIME(false);
    u8 IF = mmu.readByte(0xFF0F); // Interrupt Flag Register
    u8 IE = mmu.readByte(0xFFFF); // Interrupt Enable Register
    cpu.pushStackWord(cpu.getPC());
    
    switch (IF & IE) {
        case 0x1: { // VBlank
            printf("VBlank interrupt\n");
            mmu.writeByte(0xFF0F, IF & 0xFE);
            cpu.setPC(0x40);
            printf("PC: 0x%04X\n", cpu.getPC());
            break;
        }
        case 0x2: { // LCD
            printf("LCD interrupt\n");
            mmu.writeByte(0xFF0F, IF & 0xFD);
            cpu.setPC(0x48);
            break;
        }
        case 0x4: { // Timer
            printf("Timer interrupt\n");
            mmu.writeByte(0xFF0F, IF & 0xFB);
            cpu.setPC(0x50);
            break;
        }
        case 0x8: { // Serial
            printf("Serial interrupt\n");
            mmu.writeByte(0xFF0F, IF & 0xF7);
            cpu.setPC(0x40);
            break;
        }
        case 0xF: { // Joypad
            printf("Joypad interrupt\n");
            mmu.writeByte(0xFF0F, IF & 0xEF);
            cpu.setPC(0x60);
            break;
        }
    }
}

u8 Emulator::readMemory(u16 addr) {
    return mmu.readByte(addr);
}
