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

// CPU Instructions
int CPU::ExecuteNextOpcode( )
{
   int res = 0
   BYTE opcode = ReadMemory(m_ProgramCounter) ;
   m_ProgramCounter++ ;
   res = ExecuteOpcode(opcode) ;
   return res ;
} 

void executeOpCodes(uint16_t opcode){
    switch (opcode)
        {
            case 0x80:
                int result, carry_bit = AF.higher + BC.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x81:
                int result, carry_bit =AF.higher + BC.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x82:
                int result, carry_bit = AF.higher + DE.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x83:
                int result, carry_bit = AF.higher + DE.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x84:
                int result, carry_bit = AF.higher + HL.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x85:
                int result, carry_bit = AF.higher + HL.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x86:
                result, carry_bit = AF.higher + HL.getWord();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x87:
                result, carry_bit = AF.higher + AF.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x88:
                result, carry_bit = AF.higher + BC.higher + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x89:
                result, carry_bit = AF.higher + BC.lower + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x8A:
                result, carry_bit = AF.higher + DE.higher + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x8B:
                result, carry_bit = AF.higher + DE.lower + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x8C:
                result, carry_bit = AF.higher + HL.higher + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x8D:
                result, carry_bit = AF.higher + HL.lower + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
                break;
            case 0x8E:
                result, carry_bit = AF.higher + HL.getWord() + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
            case 0x8F:
                result, carry_bit = AF.higher + AF.lower() + CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(0);
            case 0x90:
                result, carry_bit = AF.higher - BC.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x91:
                result, carry_bit = AF.higher - BC.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x92:
                result, carry_bit = AF.higher - DE.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x93:
                result, carry_bit = AF.higher - DE.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x94:
                result, carry_bit = AF.higher - HL.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x95:
                result, carry_bit = AF.higher - HL.lower;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x96:
                result, carry_bit = AF.higher - HL.getWord();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x97:
                result, carry_bit = AF.higher - AF.higher;
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x98:
                result, carry_bit = AF.higher - BC.higher - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x99:
                result, carry_bit = AF.higher - BC.lower - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x9A:
                result, carry_bit = AF.higher + DE.higher - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x9B:
                result, carry_bit = AF.higher + DE.lower - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x9C:
                result, carry_bit = AF.higher + HL.higher - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x9D:
                result, carry_bit = AF.higher + HL.lower - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
                break;
            case 0x9E:
                result, carry_bit = AF.higher + HL.getWord() - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
            case 0x9F:
                result, carry_bit = AF.higher + AF.lower() - CPU::getCarryFlag();
                AF.higher = result;
                CPU::setZeroFlag(result==0);
                CPU::setSubFlag(1);
            case 0xA0:
                AF.higher &= BC.higher;
                break;
            case 0xA1:
                AF.higher = AF.higher and BC.lower;
                break;
            case 0xA2:
                AF.higher = AF.higher - DE.higher;
                break;
            case 0xA3:
                AF.higher = AF.higher - DE.lower;
                break;
            case 0xA4:
                AF.higher = AF.higher - HL.higher;
                break;
            case 0xA5:
                AF.higher = AF.higher - HL.lower;
                break;
            case 0xA6:
                AF.higher = AF.higher - HL.getWord();
                break;
            case 0xA7:
                AF.higher = AF.higher - AF.higher;
                break;
            case 0xA8:
                AF.higher = AF.higher - BC.higher - CPU::getCarryFlag();
                break;
            case 0xA9:
                AF.higher = AF.higher - BC.lower - CPU::getCarryFlag();
                break;
            case 0xAA:
                AF.higher = AF.higher + DE.higher - CPU::getCarryFlag();
                break;
            case 0xAB:
                AF.higher = AF.higher + DE.lower - CPU::getCarryFlag();
                break;
            case 0xAC:
                AF.higher = AF.higher + HL.higher - CPU::getCarryFlag();
                break;
            case 0xAD:
                AF.higher = AF.higher + HL.lower - CPU::getCarryFlag();
                break;
            case 0xAE:
                AF.higher = AF.higher + HL.getWord() - CPU::getCarryFlag();
            case 0xAF:
                AF.higher = AF.higher + AF.lower() - CPU::getCarryFlag();
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