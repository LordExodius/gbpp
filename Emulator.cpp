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

    int counter = 0;
    while (graphics->isOpen() && counter < 1) {
        counter++;
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear();
        window.draw(sprite);
        window.display();
        while (std::cin.get() != '\n');
        printf("Running loop\n");
        loop();
    }
}

Emulator::~Emulator() {
}

void Emulator::loop() {
    // std::fstream logfile;
    // logfile.open("log.txt", std::ios::out);
    int counter = 0;

    int cyclesPassed = 0;
    while (cyclesPassed < CYCLES_PER_FRAME && counter < 10) {
        counter++;
        u16 PC = cpu.getPC().getWord();
        // logfile <<"PC 0x" << std::hex << PC;

        u8 opCode = cpu.getInstruction();
        int cycles = cpu.executeInstruction(opCode);

        // print opcode and cycles

        // if (PC == 0x20F) {
        //     printf("\nPC: 0x%04X OPCODE: %02X CYCLES: %d\n", PC, opCode, cycles);
        //     cpu.dumpRegisters();
        // }
        
        // logfile << " OPCODE: " << std::hex << (int)opCode << " CYCLES: " << cycles << "\n";
        printf("\nPC: 0x%04X OPCODE: %02X CYCLES: %d\n", PC, opCode, cycles);

        cpu.updateTimer(cycles);
        cpu.dumpRegisters();
        cyclesPassed += cycles;
        // graphics->updateArray(cycles);
        handleInterrupts();
        while (std::cin.get() != '\n');
    }
    // graphics->updateDisplay();
    // logfile.close();
}

void Emulator::handleInterrupts() {
    if (!cpu.getIME()) {
        // printf("IME is disabled\n");
        return;
    }

    cpu.setIME(false);
    u8 IF = mmu.readByte(0xFF0F);
    u8 IE = mmu.readByte(0xFFFF);
    cpu.pushStackWord(cpu.getPC().getWord());
    
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

CPU* Emulator::getCPU() {
    return &cpu;
}

MMU* Emulator::getMMU() {
    return &mmu;
}