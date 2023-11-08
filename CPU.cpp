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

int CPU::ExecuteOpcode(BYTE opcode)
{
   switch(opcode)
   {
   case 0x06:
   CPU_8BIT_LOAD(m_RegisterBC.hi) ;
   return 8;

   case 0x80:
   CPU_8BIT_ADD(m_RegisterAF.hi, m_RegisterBC.hi,false,false) ;
   return 4;

   case 0x90:
   CPU_8BIT_SUB(m_RegisterAF.hi, m_RegisterBC.hi,false,false) ;
   return 4 ;

   case 0xAF:
   CPU_8BIT_XOR(m_RegisterAF.hi, m_RegisterAF.hi, false) ;
   return 4;

   case 0x20 :
   CPU_JUMP_IMMEDIATE( true, FLAG_Z, false ) ;
   return 8;

   case 0xCC :
   CPU_CALL( true, FLAG_Z, true) ;
   case 0xD0:
   CPU_RETURN( true, FLAG_C, false ) ;
   return 8;

   case 0xCB:
   return ExecuteExtendedOpcode( ) ;

   default:
   assert(false); return 0 ;// unhandled opcode } 

// DEBUG

void CPU::dumpRegisters() {
    std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::AF.getWord() << " (" << std::bitset<16>(CPU::AF.getWord()) << ")\n";
    std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::BC.getWord() << " (" << std::bitset<16>(CPU::BC.getWord()) << ")\n";
    std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::DE.getWord() << " (" << std::bitset<16>(CPU::DE.getWord()) << ")\n";
    std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::HL.getWord() << " (" << std::bitset<16>(CPU::HL.getWord()) << ")\n";
    std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::SP.getWord() << " (" << std::bitset<16>(CPU::SP.getWord()) << ")\n";
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::PC.getWord() << " (" << std::bitset<16>(CPU::PC.getWord()) << ")\n\n";
}