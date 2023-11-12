// Libraries
#include <iostream>

// Debug
#include <bitset>
#include <iomanip>

// Headers
#include "CPU.h"

CPU::CPU(MMU* mmu)
{
    CPU::mmu = mmu;
    // Init values from Pandocs for DMG Gameboy
    CPU::AF.setWord(0x01B0);
    CPU::BC.setWord(0x0013);    
    CPU::DE.setWord(0x00D8);
    CPU::HL.setWord(0x014D);
    CPU::SP.setWord(0xFFFE);
    CPU::PC.setWord(0x0100); // PC start location is 0x0100 for practical purposes
}

CPU::~CPU() { }

// Flag Operations

bool CPU::getZeroFlag() {
    return (CPU::AF.lower & ZERO_VALUE) == ZERO_VALUE; 
}
void CPU::setZeroFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | ZERO_VALUE : CPU::AF.lower & ~ZERO_VALUE;
}
bool CPU::getSubFlag() {
    return (CPU::AF.lower & SUB_VALUE) == SUB_VALUE; 
}
void CPU::setSubFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | SUB_VALUE : CPU::AF.lower & ~SUB_VALUE;
}
bool CPU::getHCarryFlag() {
    return (CPU::AF.lower & HALF_VALUE) == HALF_VALUE; 
}
void CPU::setHCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | HALF_VALUE : CPU::AF.lower & ~HALF_VALUE;
}
bool CPU::getCarryFlag() {
    return (CPU::AF.lower & CARRY_VALUE) == CARRY_VALUE; 
}
void CPU::setCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | CARRY_VALUE : CPU::AF.lower & ~CARRY_VALUE;
}

// Timer
u8 CPU::getDivider() {
    return mmu->readByte(DIV_ADDR);
}

u8 CPU::getTimer() {
    return mmu->readByte(TIMA_ADDR);
}

u8 CPU::getTimerModulo() {
    return mmu->readByte(TMA_ADDR);
}

void CPU::resetDivider() {
    mmu->writeByte(DIV_ADDR, 0x00);
}

void CPU::setDivider(u8 value) {
    mmu->writeByte(DIV_ADDR, value);
}

void CPU::setTimer(u8 value) {
    mmu->writeByte(TIMA_ADDR, value);
}

void CPU::setTimerModulo(u8 value) {
    mmu->writeByte(TMA_ADDR, value);
}

void CPU::updateTimer(int instructionCycles) { // M CYCLES
    // Update DIV before timer, independent of timer control
    int cpuCycles = 4 * instructionCycles; // T cycles

    divCounter += cpuCycles;
    while (divCounter >= (CPU_CLOCK_SPEED/DIV_SPEED)) { // If enough CPU cycles have passed, (256) increment divider
        u8 divider = getDivider();
        if (divider == 0xFF) // Reset once divider hits 255
            resetDivider();
        else 
            setDivider(divider + 1);

        divCounter -= CPU_CLOCK_SPEED/DIV_SPEED;
    }
    
    u8 TAC = mmu->readByte(TAC_ADDR); // Get TAC byte
    if (TAC & 0x04 == 0x00) return; // do not do anything if timer is disabled

    int frequency; // Frequency = bits 1 and 0 of TAC
    switch (TAC & 0x03) {
        case 0x00: 
            frequency = TAC_0;
            break;
        case 0x01:
            frequency = TAC_1;
            break;
        case 0x02:
            frequency = TAC_2;
            break;
        case 0x03:
            frequency = TAC_3;
            break;
        default:
            frequency = TAC_0;
            break;
    }

    timerCounter += cpuCycles;
    while (timerCounter >= CPU_CLOCK_SPEED/frequency) {
        u8 TIMA = getTimer();
        if (TIMA == 0xFF) {
            setTimer(getTimerModulo());
            // SET INTERRUPT
        }
        timerCounter -= CPU_CLOCK_SPEED/frequency;
    }
}

// DEBUG

void CPU::dumpRegisters() {
    std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::AF.getWord() << " (" << std::bitset<16>(CPU::AF.getWord()) << ")\n";
    std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::BC.getWord() << " (" << std::bitset<16>(CPU::BC.getWord()) << ")\n";
    std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::DE.getWord() << " (" << std::bitset<16>(CPU::DE.getWord()) << ")\n";
    std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::HL.getWord() << " (" << std::bitset<16>(CPU::HL.getWord()) << ")\n";
    std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::SP.getWord() << " (" << std::bitset<16>(CPU::SP.getWord()) << ")\n";
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::PC.getWord() << " (" << std::bitset<16>(CPU::PC.getWord()) << ")\n\n";
}