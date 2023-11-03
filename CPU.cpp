// Libraries
#include <iostream>

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
    return (CPU::AF.lower & 0b0001) == 0b0001; 
}
void CPU::setZeroFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | 0b0001 : CPU::AF.lower & 0b1110;
}
bool CPU::getSubFlag() {
    return (CPU::AF.lower & 0b0010) == 0b0010; 
}
void CPU::setSubFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | 0b0010 : CPU::AF.lower & 0b1101;
}
bool CPU::getHCarryFlag() {
    return (CPU::AF.lower & 0b0100) == 0b0100; 
}
void CPU::setHCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | 0b1000 : CPU::AF.lower & 0b1011;
}
bool CPU::getCarryFlag() {
    return (CPU::AF.lower & 0b1000) == 0b1000; 
}
void CPU::setCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | 0b1000 : CPU::AF.lower & 0b0111;
}
