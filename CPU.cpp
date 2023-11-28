// Libraries
#include <iostream>

// Debug
#include <bitset>
#include <iomanip>

// Headers
#include "CPU.h"

CPU::CPU(MMU *mmu)
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

CPU::~CPU() {}

// Flag Operations

bool CPU::getZeroFlag() {
    return CPU::AF.lower & ZERO_VALUE; 
}
void CPU::setZeroFlag(bool set)
{
    CPU::AF.lower = set ? CPU::AF.lower | ZERO_VALUE : CPU::AF.lower & ~ZERO_VALUE;
}
bool CPU::getSubFlag() {
    return CPU::AF.lower & SUB_VALUE; 
}
void CPU::setSubFlag(bool set)
{
    CPU::AF.lower = set ? CPU::AF.lower | SUB_VALUE : CPU::AF.lower & ~SUB_VALUE;
}
bool CPU::getHCarryFlag() {
    return CPU::AF.lower & HALF_VALUE; 
}
void CPU::setHCarryFlag(bool set)
{
    CPU::AF.lower = set ? CPU::AF.lower | HALF_VALUE : CPU::AF.lower & ~HALF_VALUE;
}
bool CPU::getCarryFlag() {
    return CPU::AF.lower & CARRY_VALUE; 
}
void CPU::setCarryFlag(bool set)
{
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
u8 CPU::getDivider()
{
    return mmu->readByte(DIV_ADDR);
}

u8 CPU::getTimer()
{
    return mmu->readByte(TIMA_ADDR);
}

u8 CPU::getTimerModulo()
{
    return mmu->readByte(TMA_ADDR);
}

void CPU::resetDivider()
{
    mmu->writeByte(DIV_ADDR, 0x00);
}

void CPU::setDivider(u8 value)
{
    mmu->writeByte(DIV_ADDR, value);
}

void CPU::setTimer(u8 value)
{
    mmu->writeByte(TIMA_ADDR, value);
}

void CPU::setTimerModulo(u8 value)
{
    mmu->writeByte(TMA_ADDR, value);
}

void CPU::updateTimer(int instructionCycles)
{ // M CYCLES
    // Update DIV before timer, independent of timer control
    int cpuCycles = 4 * instructionCycles; // T cycles

    divCounter += cpuCycles;
    while (divCounter >= (CPU_CLOCK_SPEED / DIV_SPEED))
    { // If enough CPU cycles have passed, (256) increment divider
        u8 divider = getDivider();
        if (divider == 0xFF) // Reset once divider hits 255
            resetDivider();
        else
            setDivider(divider + 1);

        divCounter -= CPU_CLOCK_SPEED / DIV_SPEED;
    }

    u8 TAC = mmu->readByte(TAC_ADDR); // Get TAC byte
    if (TAC & 0x04 == 0x00)
        return; // do not do anything if timer is disabled

    // Frequency of TIMA is determined by bits 1 and 0 of TAC
    int frequency; 
    switch (TAC & 0x03)
    {
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
    while (timerCounter >= CPU_CLOCK_SPEED / frequency)
    {
        u8 TIMA = getTimer();
        if (TIMA == 0xFF)
        {
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

int CPU::executeInstruction(u8 instruction)
{
    switch (instruction)
    {
    // NOOP
    case 0x00:
        return 1;
    // STPO ? WTF
    case 0x10:
        return 1;
    // JR NZ i8?
    case 0x20:
        return 1;
    // JR NC i8?
    case 0x30:
        return 1;
    // LD B, B
    case 0x40:
        BC.lower = BC.lower;
        return 1;
    // LD D, B
    case 0x50:
        DE.lower = BC.lower;
        return 1;
    // LD H, B
    case 0x60:
        HL.lower = BC.lower;
        return 1;
    // LD (HL), B
    case 0x70:
        mmu->writeWord(HL.getWord(), BC.lower);
        return 2;
    // LD BC,u16
    case 0x01:
        BC.lower = mmu->readByte(PC.getWord() + 1);
        BC.higher = mmu->readByte(PC.getWord() + 2);
        return 3;
    // LD DE, u16
    case 0x11:
        DE.lower = mmu->readByte(DE.getWord() + 1);
        DE.higher = mmu->readByte(DE.getWord() + 2);
        return 3;
    // LD HL, u16
    case 0x21:
        HL.lower = mmu->readByte(HL.getWord() + 1);
        HL.higher = mmu->readByte(HL.getWord() + 2);
        return 3;
    // LD SP, u16
    case 0x31:
        SP.lower = mmu->readByte(SP.getWord() + 1);
        SP.higher = mmu->readByte(SP.getWord() + 2);
        return 3;
    // LD B, C
    case 0x41:
        BC.lower = BC.higher;
        return 1;
    // LD D, C
    case 0x51:
        DE.lower = BC.higher;
        return 1;
    // LD H, C
    case 0x61:
        HL.lower = BC.higher;
        return 1;
    // LD (HL), C
    case 0x71:
        mmu->writeWord(HL.getWord(), BC.higher);
        return 2;
    // LD (BC),A
    case 0x02:
        mmu->writeWord(HL.getWord(), AF.lower);
        return 2;
    // LD (DE),A
    case 0x12:
        mmu->writeWord(DE.getWord(), AF.lower);
        return 2;
    // LD (HL+),A
    case 0x22:
        mmu->writeWord(HL.getWord(), AF.lower);
        HL.setWord(HL.getWord() + 1);
        return 2;
    // LD (HL-),A
    case 0x32:
        mmu->writeWord(HL.getWord(), AF.lower);
        HL.setWord(HL.getWord() - 1);
        return 2;
    // LD B, D
    case 0x42:
        BC.lower = DE.lower;
        return 1;
    // LD D, D
    case 0x52:
        DE.lower = DE.lower;
        return 1;
    // LD H, D
    case 0x62:
        HL.lower = DE.lower;
        return 1;
    // LD (HL), D
    case 0x72:
        mmu->writeWord(HL.getWord(), DE.lower);
        return 2;
    // INC BC ? should be 16 bit operation:
    case 0x03:
        int res, carry_bit = BC.getWord() + 1;
        BC.lower = res;
        CPU::setZeroFlag(BC.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 3;
    // INC DE
    case 0x13:
        int res, carry_bit = DE.getWord() + 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 3;
    // INC HL
    case 0x23:
        int res, carry_bit = HL.getWord() + 1;
        HL.lower = res;
        CPU::setZeroFlag(HL.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 3;
    // INC SP
    case 0x33:
        int res, carry_bit = DE.getWord() + 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 3;
    // LD B, E
    case 0x43:
        BC.lower = DE.higher;
        return 1;
    // LD D, D
    case 0x53:
        DE.lower = DE.higher;
        return 1;
    // LD H, D
    case 0x63:
        HL.lower = DE.higher;
        return 1;
    // LD (HL), D
    case 0x73:
        mmu->writeWord(HL.getWord(), DE.higher);
        return 2;
    // INC B
    case 0x04:
        int res, carry_bit = BC.lower + 1;
        BC.lower = res;
        CPU::setZeroFlag(BC.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC D
    case 0x14:
        int res, carry_bit = DE.lower + 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC H
    case 0x24:
        int res, carry_bit = HL.lower + 1;
        HL.lower = res;
        CPU::setZeroFlag(HL.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC (HL)
    case 0x34:
        int res, carry_bit = mmu->readWord(HL.getWord()) + 1;
        mmu->writeWord(HL.getWord(), res);
        CPU::setZeroFlag(res == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // LD B, H
    case 0x44:
        BC.lower = HL.lower;
        return 1;
    // LD D, H
    case 0x54:
        DE.lower = HL.lower;
        return 1;
    // LD H, H
    case 0x64:
        HL.lower = HL.lower;
        return 1;
    // LD (HL), H
    case 0x74:
        mmu->writeWord(HL.getWord(), HL.lower);
        return 2;
    // DEC B
    case 0x05:
        int res, carry_bit = BC.lower - 1;
        BC.lower = res;
        CPU::setZeroFlag(BC.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // DEC D
    case 0x15:
        int res, carry_bit = DE.lower - 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // DEC H
    case 0x25:
        int res, carry_bit = HL.lower - 1;
        HL.lower = res;
        CPU::setZeroFlag(HL.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // DEC (HL)
    case 0x35:
        int res, carry_bit = mmu->readWord(HL.getWord()) - 1;
        mmu->writeWord(HL.getWord(), res);
        CPU::setZeroFlag(res == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // LD B, L
    case 0x45:
        BC.lower = HL.higher;
        return 1;
    // LD D, L
    case 0x55:
        DE.lower = HL.higher;
        return 1;
    // LD H, L
    case 0x65:
        HL.lower = HL.higher;
        return 1;
    // LD (HL), L
    case 0x75:
        mmu->writeWord(HL.getWord(), HL.higher);
        return 2;
    // LD B,u8
    case 0x06:
        BC.lower = mmu->readByte(PC.getWord() + 1);
        return 2;
    // RLCA
    case 0x07:
        return 0;
    // LD B, A
    case 0x47:
        BC.lower = AF.lower;
        return 1;
    // LD D, A
    case 0x57:
        DE.lower = AF.lower;
        return 1;
    // LD H, A
    case 0x67:
        HL.lower = AF.lower;
        return 1;
    // LD (HL), A
    case 0x77:
        mmu->writeWord(HL.getWord(), AF.lower);
        return 2;
    case 0x08:
        u16 nn = mmu->readWord(PC.getWord() + 1);
        SP.setWord(nn);
        return 5;
    // LD C, B
    case 0x48:
        BC.higher = BC.lower;
        return 1;
    // LD E, B
    case 0x58:
        DE.higher = BC.lower;
        return 1;
    // LD L, B
    case 0x68:
        HL.higher = BC.lower;
        return 1;
    // LD A, B
    case 0x78:
        AF.lower = BC.lower;
        return 1;
    // ADD HL,BC ? is machine cycle 2?
    case 0x09:
        u16 nn = HL.getWord() + BC.getWord();
        HL.setWord(nn);
        CPU::setZeroFlag(HL.getWord() == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(nn);
        return 2;
    // ADD HL,DE
    case 0x19:
        u16 nn = HL.getWord() + DE.getWord();
        HL.setWord(nn);
        CPU::setZeroFlag(HL.getWord() == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(nn);
        return 2;
    // ADD HL,HL
    case 0x29:
        u16 nn = HL.getWord() + HL.getWord();
        HL.setWord(nn);
        CPU::setZeroFlag(HL.getWord() == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(nn);
        return 2;
    // ADD HL,SP
    case 0x39:
        u16 nn = HL.getWord() + SP.getWord();
        HL.setWord(nn);
        CPU::setZeroFlag(HL.getWord() == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(nn);
        return 2;
    // LD C, C
    case 0x49:
        BC.higher = BC.higher;
        return 1;
    // LD E, C
    case 0x59:
        DE.higher = BC.higher;
        return 1;
    // LD L, C
    case 0x69:
        HL.higher = BC.higher;
        return 1;
    // LD A, C
    case 0x79:
        AF.lower = BC.higher;
        return 1;
    // LD C, D
    case 0x4A:
        BC.higher = DE.lower;
        return 1;
    // LD E, D
    case 0x5A:
        DE.higher = DE.lower;
        return 1;
    // LD L, D
    case 0x6A:
        HL.higher = DE.lower;
        return 1;
    // LD A, D
    case 0x7A:
        AF.lower = DE.lower;
        return 1;
    // DEC BC
    case 0x0B:
        u16 res, carry_bit = BC.getWord() - 1;
        BC.setWord(res);
        CPU::setZeroFlag(BC.getWord() == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 3;
    // DEC DE
    case 0x1B:
        u16 res, carry_bit = DE.getWord() - 1;
        DE.setWord(res);
        CPU::setZeroFlag(DE.getWord() == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 3;
    // DEC HL
    case 0x2B:
        u16 res, carry_bit = HL.getWord() - 1;
        HL.setWord(res);
        CPU::setZeroFlag(HL.getWord() == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 3;
    // DEC SP
    case 0x3B:
        u16 res, carry_bit = SP.getWord() - 1;
        SP.setWord(res);
        CPU::setZeroFlag(SP.getWord() == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 3;
    // LD C, E
    case 0x4B:
        BC.higher = DE.higher;
        return 1;
    // LD E, E
    case 0x5B:
        DE.higher = DE.higher;
        return 1;
    // LD L, E
    case 0x6B:
        HL.higher = DE.higher;
        return 1;
    // LD A, D
    case 0x7B:
        AF.lower = DE.higher;
        return 1;
    // INC C
    case 0x0C:
        u8 res = BC.lower + 1;
        BC.lower = res;
        CPU::setZeroFlag(BC.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC E
    case 0x1C:
        u8 res = DE.lower + 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC L
    case 0x2C:
        u8 res = HL.lower + 1;
        HL.lower = res;
        CPU::setZeroFlag(HL.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // INC A
    case 0x3C:
        u8 res = AF.lower + 1;
        AF.lower = res;
        CPU::setZeroFlag(AF.lower == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        return 1;
    // LD C, H
    case 0x4C:
        BC.higher = HL.lower;
        return 1;
    // LD E, H
    case 0x5C:
        DE.higher = HL.lower;
        return 1;
    // LD L, H
    case 0x6C:
        HL.higher = HL.lower;
        return 1;
    // LD A, H
    case 0x7C:
        AF.lower = HL.lower;
        return 1;
    // DEC C
    case 0x0D:
        u8 res = BC.lower - 1;
        BC.lower = res;
        CPU::setZeroFlag(BC.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // INC E
    case 0x1D:
        u8 res = DE.lower - 1;
        DE.lower = res;
        CPU::setZeroFlag(DE.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // INC L
    case 0x2D:
        u8 res = HL.lower - 1;
        HL.lower = res;
        CPU::setZeroFlag(HL.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // INC A
    case 0x3D:
        u8 res = AF.lower - 1;
        AF.lower = res;
        CPU::setZeroFlag(AF.lower == 0);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(res);
        return 1;
    // LD C, L
    case 0x4D:
        BC.higher = HL.higher;
        return 1;
    // LD E, L
    case 0x5D:
        DE.higher = HL.higher;
        return 1;
    // LD L, L
    case 0x6D:
        HL.higher = HL.higher;
        return 1;
    // LD A, L
    case 0x7D:
        AF.lower = HL.higher;
        return 1;
    // LD C, (HL)
    case 0x4E:
        BC.higher = mmu->readByte(HL.getWord());
        return 1;
    // LD E, (HL)
    case 0x5E:
        DE.higher = mmu->readByte(HL.getWord());
        return 1;
    // LD L, (HL)
    case 0x6E:
        HL.higher = mmu->readByte(HL.getWord());
        return 1;
    // LD A, (HL)
    case 0x7E:
        AF.lower = mmu->readByte(HL.getWord());
        return 1;
    // LD C, A
    case 0x4F:
        BC.higher = AF.lower;
        return 1;
    // LD E, A
    case 0x5F:
        DE.higher = AF.lower;
        return 1;
    // LD L, A
    case 0x6F:
        HL.higher = AF.lower;
        return 1;
    // LD A, A
    case 0x7F:
        AF.lower = AF.lower;
        return 1;
    // ADD A, B
    case 0x80:
        u8 res = AF.higher + BC.higher;
        CPU::setZeroFlag(AF.higher == 0);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(res);
        AF.higher = res;
        return 1;
        break;
    // ADD A, C
    case 0x81:
        int result, carry_bit = AF.higher + BC.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, D
    case 0x82:
        int result, carry_bit = AF.higher + DE.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, E
    case 0x83:
        int result, carry_bit = AF.higher + DE.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, H
    case 0x84:
        int result, carry_bit = AF.higher + HL.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, L
    case 0x85:
        int result, carry_bit = AF.higher + HL.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, (HL)
    // Adds to the 8-bit A register,
    // data from the absolute address specified by the 16-bit register HL,
    // stores result back into the A register.
    case 0x86:
        result, carry_bit = AF.higher + HL.getWord();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADD A, A
    case 0x87:
        result, carry_bit = AF.higher + AF.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, B
    case 0x88:
        result, carry_bit = AF.higher + BC.higher + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, C
    case 0x89:
        result, carry_bit = AF.higher + BC.lower + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, D
    case 0x8A:
        result, carry_bit = AF.higher + DE.higher + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, E
    case 0x8B:
        result, carry_bit = AF.higher + DE.lower + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, H
    case 0x8C:
        result, carry_bit = AF.higher + HL.higher + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, L
    case 0x8D:
        result, carry_bit = AF.higher + HL.lower + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
        break;
    // ADC A, (HL)
    // Adds to the 8-bit A register,
    // the carry flag and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x8E:
        result, carry_bit = AF.higher + HL.getWord() + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
    // ADC A, A
    case 0x8F:
        result, carry_bit = AF.higher + AF.lower() + CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(0);
    // SUB A, B
    case 0x90:
        result, carry_bit = AF.higher - BC.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, C
    case 0x91:
        result, carry_bit = AF.higher - BC.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, D
    case 0x92:
        result, carry_bit = AF.higher - DE.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, E
    case 0x93:
        result, carry_bit = AF.higher - DE.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, H
    case 0x94:
        result, carry_bit = AF.higher - HL.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, L
    case 0x95:
        result, carry_bit = AF.higher - HL.lower;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, (HL)
    // Subtracts from the 8-bit A register,
    // data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x96:
        result, carry_bit = AF.higher - HL.getWord();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SUB A, A
    case 0x97:
        result, carry_bit = AF.higher - AF.higher;
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, B
    case 0x98:
        result, carry_bit = AF.higher - BC.higher - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, C
    case 0x99:
        result, carry_bit = AF.higher - BC.lower - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, D
    case 0x9A:
        result, carry_bit = AF.higher + DE.higher - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, E
    case 0x9B:
        result, carry_bit = AF.higher + DE.lower - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, H
    case 0x9C:
        result, carry_bit = AF.higher + HL.higher - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, L
    case 0x9D:
        result, carry_bit = AF.higher + HL.lower - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
        break;
    // SBC A, (HL)
    // Subtracts from the 8-bit A register,
    // the carry flag and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x9E:
        result, carry_bit = AF.higher + HL.getWord() - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
    // SBC A, A
    case 0x9F:
        result, carry_bit = AF.higher + AF.lower() - CPU::getCarryFlag();
        AF.higher = result;
        CPU::setZeroFlag(result == 0);
        CPU::setSubFlag(1);
    // AND A, B: Bitwise AND
    case 0xA0:
        AF.higher &= BC.higher;
        break;
    // AND A, C
    case 0xA1:
        AF.higher = AF.higher and BC.lower;
        break;
    // AND A, D
    case 0xA2:
        AF.higher = AF.higher - DE.higher;
        break;
    // AND A, E
    case 0xA3:
        AF.higher = AF.higher - DE.lower;
        break;
    // AND A, H
    case 0xA4:
        AF.higher = AF.higher - HL.higher;
        break;
    // AND A, L
    case 0xA5:
        AF.higher = AF.higher - HL.lower;
        break;
    // AND A, (HL)
    // Performs a bitwise AND operation between
    // the 8-bit A register and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0xA6:
        AF.higher = AF.higher - HL.getWord();
        break;
    // AND A, A
    case 0xA7:
        AF.higher = AF.higher - AF.higher;
        break;
    // XOR A, B
    case 0xA8:
        AF.higher = AF.higher - BC.higher - CPU::getCarryFlag();
        break;
    // XOR A, C
    case 0xA9:
        AF.higher = AF.higher - BC.lower - CPU::getCarryFlag();
        break;
    // XOR A, D
    case 0xAA:
        AF.higher = AF.higher + DE.higher - CPU::getCarryFlag();
        break;
    // XOR A, E
    case 0xAB:
        AF.higher = AF.higher + DE.lower - CPU::getCarryFlag();
        break;
    // XOR A, H
    case 0xAC:
        AF.higher = AF.higher + HL.higher - CPU::getCarryFlag();
        break;
    // XOR A, L
    case 0xAD:
        AF.higher = AF.higher + HL.lower - CPU::getCarryFlag();
        break;
    // XOR (HL)
    // Performs a bitwise XOR operation between
    // the 8-bit A register and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0xAE:
        AF.higher = AF.higher + HL.getWord() - CPU::getCarryFlag();
    case 0xAF:
        AF.higher = AF.higher + AF.lower() - CPU::getCarryFlag();
    }
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

void CPU::dumpRegisters()
{
    std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::AF.getWord() << " (" << std::bitset<16>(CPU::AF.getWord()) << ")\n";
    std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::BC.getWord() << " (" << std::bitset<16>(CPU::BC.getWord()) << ")\n";
    std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::DE.getWord() << " (" << std::bitset<16>(CPU::DE.getWord()) << ")\n";
    std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::HL.getWord() << " (" << std::bitset<16>(CPU::HL.getWord()) << ")\n";
    std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::SP.getWord() << " (" << std::bitset<16>(CPU::SP.getWord()) << ")\n";
    std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::PC.getWord() << " (" << std::bitset<16>(CPU::PC.getWord()) << ")\n\n";
}