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
    return CPU::AF.lower & ZERO_VALUE; 
}
void CPU::setZeroFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | ZERO_VALUE : CPU::AF.lower & ~ZERO_VALUE;
}
bool CPU::getSubFlag() {
    return CPU::AF.lower & SUB_VALUE; 
}
void CPU::setSubFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | SUB_VALUE : CPU::AF.lower & ~SUB_VALUE;
}
bool CPU::getHCarryFlag() {
    return CPU::AF.lower & HALF_VALUE; 
}
void CPU::setHCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | HALF_VALUE : CPU::AF.lower & ~HALF_VALUE;
}
bool CPU::getCarryFlag() {
    return CPU::AF.lower & CARRY_VALUE; 
}
void CPU::setCarryFlag(bool set) {
    CPU::AF.lower = set ? CPU::AF.lower | CARRY_VALUE : CPU::AF.lower & ~CARRY_VALUE;
}

// Flag Helpers
bool CPU::checkHCarry_8(u8 arg1, u8 arg2, u8 res) {
    return ((arg1 ^ arg2 ^ res) & 0x10);
}
bool CPU::checkHCarry_16(u16 arg1, u16 arg2, u16 res) {
    return ((arg1 ^ arg2 ^ res) & 0x1000);
}
bool CPU::checkCarry_8(u8 arg1, u8 arg2) {
    return ( static_cast<u16> (arg1) + static_cast<u16> (arg2) > 0xFF );
}
bool CPU::checkCarry_16(u16 arg1, u16 arg2) {
    return ( static_cast<u32> (arg1) + static_cast<u32> (arg2) > 0xFFFF );
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

    // Frequency of TIMA is determined by bits 1 and 0 of TAC
    int frequency; 
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
            // [TODO] SET INTERRUPT
        }
        timerCounter -= CPU_CLOCK_SPEED/frequency;
    }
}

u8 CPU::getInstruction() {
    u8 opcode = mmu->readByte(PC.getWord());
    // update PC
    PC.setWord(PC.getWord() + 1);
    return opcode;
}

int CPU::executeInstruction(u8 instruction){
    // switch (instruction)
    //     {
    //         // ADD A, B
    //         case 0x80: {
    //             u8 res = AF.higher + BC.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.higher, res));
    //             // CPU::setCarryFlag(CPU::)
    //             AF.higher = res;
    //             return 1;
    //         }

    //         // ADD A, C
    //         case 0x81: {
    //             u8 res = AF.higher + BC.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         }

    //        // ADD A, D
    //         case 0x82: {
    //             u8 res = AF.higher + DE.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, DE.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         }
    //         // ADD A, E
    //         case 0x83: {
    //             u8 res = AF.higher + DE.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, DE.lower, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADD A, H
    //         case 0x84: {
    //             u8 res = AF.higher + HL.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, HL.higher, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADD A, L
    //         case 0x85: {
    //             u8 res = AF.higher + HL.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, HL.lower, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADD A, (HL)
    //         // Adds to the 8-bit A register, 
    //         // data from the absolute address specified by the 16-bit register HL, 
    //         // stores result back into the A register.
    //         case 0x86: {
    //             u16 res = AF.higher + mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, mmu->readByte(HL.getWord()), res));
    //             AF.higher = res;
    //             return 2;
    //             }
    //         // ADD A, A
    //         case 0x87: {
    //             u8 res = AF.higher + AF.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, AF.higher, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADC A, B
    //         case 0x88: {
    //             u8 res = AF.higher + BC.higher + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.higher, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADC A, C
    //         case 0x89: {
    //             u8 res = AF.higher + BC.lower + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,BC.lower, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADC A, D
    //         case 0x8A: {
    //             u8 res = AF.higher + DE.higher + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         }
    //         // ADC A, E
    //         case 0x8B: {
    //             u8 res = AF.higher + DE.lower + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.lower, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADC A, H
    //         case 0x8C: {
    //             u8 res = AF.higher + HL.higher + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.higher, res));
    //             AF.higher = res;
    //             return 1;
    //             }
    //         // ADC A, L
    //         case 0x8D:
    //             u8 res = AF.higher + HL.lower + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // ADC A, (HL)
    //         // Adds to the 8-bit A register, 
    //         // the carry flag and data from the absolute address specified by the 16-bit register HL, 
    //         // and stores the result back into the A register.
    //         case 0x8E:
    //             u16 res = AF.higher + HL.getWord() + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_16(AF.higher,HL.getWord(), res));
    //             AF.higher = res;
    //             return 2;
    //         // ADC A, A
    //         case 0x8F:
    //             u8 res = AF.higher + AF.lower + CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, AF.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, B
    //         case 0x90:
    //             u8 res = AF.higher - BC.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, C
    //         case 0x91:
    //             u8 res = AF.higher - BC.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,BC.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, D
    //         case 0x92:
    //             u8 res = AF.higher - DE.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, E
    //         case 0x93:
    //             u8 res = AF.higher - DE.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, H
    //         case 0x94:
    //             u8 res = AF.higher - HL.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, L
    //         case 0x95:
    //             u8 res = AF.higher - HL.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SUB A, (HL)
    //         // Subtracts from the 8-bit A register, 
    //         // data from the absolute address specified by the 16-bit register HL, 
    //         // and stores the result back into the A register.
    //         case 0x96:
    //             u16 res = AF.higher - mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_16(AF.higher,HL.getWord(), res));
    //             AF.higher = res;
    //             return 2;
    //         // SUB A, A
    //         case 0x97:
    //             u8 res = AF.higher - AF.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(res);
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, B
    //         case 0x98:
    //             u8 res = AF.higher - BC.higher - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,BC.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, C
    //         case 0x99:
    //             u8 res = AF.higher - BC.lower - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,BC.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, D
    //         case 0x9A:
    //             u8 res = AF.higher - DE.higher - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, E
    //         case 0x9B:
    //             u8 res = AF.higher - DE.lower - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,DE.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, H
    //         case 0x9C:
    //             u8 res = AF.higher - HL.higher - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.higher, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, L
    //         case 0x9D:
    //             u8 res = AF.higher - HL.lower - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,HL.lower, res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, (HL)
    //         // Subtracts from the 8-bit A register, 
    //         // the carry flag and data from the absolute address specified by the 16-bit register HL, 
    //         // and stores the result back into the A register.
    //         case 0x9E:
    //             u8 res = AF.higher - mmu->readByte(HL.getWord()) - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,mmu->readByte(HL.getWord()), res));
    //             AF.higher = res;
    //             return 1;
    //         // SBC A, A
    //         case 0x9F:
    //             u8 res = AF.higher + AF.higher - CPU::getCarryFlag();
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher,mmu->readByte(HL.getWord()), res));
    //             AF.higher = res;
    //             return 1;
    //         // AND A, B: Bitwise AND 
    //         case 0xA0:
    //             AF.higher &= BC.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, C
    //         case 0xA1:
    //             AF.higher &= BC.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, D
    //         case 0xA2:
    //             AF.higher &= DE.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, E
    //         case 0xA3:
    //             AF.higher &= DE.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, H
    //         case 0xA4:
    //             AF.higher &= HL.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, L
    //         case 0xA5:
    //             AF.higher &= HL.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, (HL)
    //         // Performs a bitwise AND operation between 
    //         // the 8-bit A register and data from the absolute address specified by the 16-bit register HL, 
    //         // and stores the result back into the A register.
    //         case 0xA6:
    //             AF.higher &= mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // AND A, A
    //         case 0xA7:
    //             AF.higher &= AF.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(1);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, B
    //         case 0xA8:
    //             AF.higher = AF.higher ^ BC.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, C
    //         case 0xA9:
    //             AF.higher = AF.higher ^ BC.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, D
    //         case 0xAA:
    //             AF.higher = AF.higher ^ DE.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, E
    //         case 0xAB:
    //             AF.higher = AF.higher ^ DE.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, H
    //         case 0xAC:
    //             AF.higher = AF.higher ^ HL.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR A, L
    //         case 0xAD:
    //             AF.higher = AF.higher ^ HL.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // XOR (HL)
    //         // Performs a bitwise XOR operation between 
    //         // the 8-bit A register and data from the absolute address specified by the 16-bit register HL, 
    //         // and stores the result back into the A register.
    //         case 0xAE:
    //             AF.higher = AF.higher ^ mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 2;
    //         // XOR A, A
    //         case 0xAF:
    //             AF.higher = AF.higher ^ AF.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, B
    //         case 0xB0:
    //             AF.higher = AF.higher | BC.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, C
    //         case 0xB1:
    //             AF.higher = AF.higher | BC.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, D
    //         case 0xB2:
    //             AF.higher = AF.higher | DE.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, E
    //         case 0xB3:
    //             AF.higher = AF.higher | DE.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, H
    //         case 0xB4:
    //             AF.higher = AF.higher | HL.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // Bitwise OR A, L
    //         case 0xB5:
    //             AF.higher = AF.higher | HL.lower;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // OR (HL): Bitwise OR (indirect HL)
    //         case 0xB6:
    //             AF.higher = AF.higher | mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 2;
    //         // Bitwise OR A, A
    //         case 0xB7:
    //             AF.higher = AF.higher | AF.higher;
    //             CPU::setZeroFlag(AF.higher==0);
    //             CPU::setSubFlag(0);
    //             CPU::setHCarryFlag(0);
    //             CPU::setCarryFlag(0);
    //             return 1;
    //         // CP: A, B
    //         case 0xB8:
    //             res = AF.higher - BC.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.higher, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, C
    //         case 0xB9:
    //             res = AF.higher - BC.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.lower, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, D
    //         case 0xBA:
    //             res = AF.higher - DE.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, DE.higher, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, E
    //         case 0xBB:
    //             res = AF.higher - DE.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, DE.lower, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, H
    //         case 0xBC:
    //             res = AF.higher - HL.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, HL.higher, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, L
    //         case 0xBD:
    //             res = AF.higher - HL.lower;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, HL.lower, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, (HL)
    //         case 0xBE:
    //             res = AF.higher - CPU::mmu->readByte(HL.getWord());
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, HL.lower, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //         // CP: A, A
    //         case 0xBF:
    //             res = AF.higher - AF.higher;
    //             CPU::setZeroFlag(!res);
    //             CPU::setSubFlag(1);
    //             CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, AF.higher, res));
    //             CPU::setCarryFlag(1);
    //             AF.higher = res;
    //             return 1;
    //     }
}

void CPU::add_a(u8 arg) {
    u16 res = AF.higher + arg;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(false);
    CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, BC.higher, res));
    // CPU::setCarryFlag(CPU::) 
    AF.higher = res;
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