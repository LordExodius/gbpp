#include "Emulator.h"
#include <iostream>
#include <fstream>

Emulator::Emulator(const char *fileName): cartridge(fileName), mmu(&cartridge, fileName), cpu(&mmu) {
    printf("Loading %s\n", fileName);
    cpu.dumpRegisters();
    window.create(sf::VideoMode(160, 144), "Gameboy Emulator");
}

void Emulator::run() {
    graphics = new Graphics(&mmu, &cpu, &window);

    sf::Texture texture;
    if (!texture.loadFromFile("nintendo.png")) {
        std::cerr << "Failed to load texture from file." << std::endl;
    }
    texture.loadFromFile("nintendo.png");
    sf::Sprite sprite(texture);

    while (graphics->isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
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

        u8 opCode = cpu.getInstruction();
        int cycles = cpu.executeInstruction(opCode);

        // print opcode and cycles
        printf("PC: 0x%04X OPCODE: %02X CYCLES: %d\n", PC, opCode, cycles);
        logfile << " OPCODE: " << std::hex << (int)opCode << " CYCLES: " << cycles << "\n";

        cpu.updateTimer(cycles);
        cyclesPassed += cycles;
        while (std::cin.get() != '\n');
        graphics->updateArray(cycles);
        handleInterrupts();
    }
    graphics->updateDisplay();
    logfile.close();
}

void Emulator::handleInterrupts() {
    if (!cpu.getIME()) {
        return;
    }

    cpu.setIME(false);
    u8 IF = mmu.readByte(0xFF0F);
    u8 IE = mmu.readByte(0xFFFF);
    cpu.pushStackWord(cpu.getPC());
    
    switch (IF & IE) {
        case 0x1: { // VBlank
            mmu.writeByte(0xFF0F, IF & 0xFE);
            cpu.setPC(0x40);
            break;
        }
        case 0x2: { // LCD
            mmu.writeByte(0xFF0F, IF & 0xFD);
            cpu.setPC(0x48);
            break;
        }
        case 0x4: { // Timer
            mmu.writeByte(0xFF0F, IF & 0xFB);
            cpu.setPC(0x50);
            break;
        }
        case 0x8: { // Serial
            mmu.writeByte(0xFF0F, IF & 0xF7);
            cpu.setPC(0x40);
            break;
        }
        case 0xF: { // Joypad
            mmu.writeByte(0xFF0F, IF & 0xEF);
            cpu.setPC(0x60);
            break;
        }
    }
}

u8 Emulator::readMemory(u16 addr) {
    return mmu.readByte(addr);
}

CPU* Emulator::getCPU() {
    return &cpu;
}
