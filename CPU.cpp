// Libraries
#include <iostream>

// Debug
#include <bitset>
#include <iomanip>

// Headers
#include "global.h"
#include "CPU.h"

CPU::CPU(/* args */)
{
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

// DEBUG

void CPU::dumpRegisters() {
    std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::AF.getWord() << " (" << std::bitset<16>(CPU::AF.getWord()) << ")\n";
    std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::BC.getWord() << " (" << std::bitset<16>(CPU::BC.getWord()) << ")\n";
    std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::DE.getWord() << " (" << std::bitset<16>(CPU::DE.getWord()) << ")\n";
    std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::HL.getWord() << " (" << std::bitset<16>(CPU::HL.getWord()) << ")\n";
    std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::SP.getWord() << " (" << std::bitset<16>(CPU::SP.getWord()) << ")\n";
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::PC.getWord() << " (" << std::bitset<16>(CPU::PC.getWord()) << ")\n\n";
}