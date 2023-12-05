// Libraries
#include <iostream>

// Debug
#include <bitset>
#include <iomanip>

// Headers
#include "CPU.h"

CPU::CPU(MMU *mmu)
{
    // printf("Initializing CPU...\n");
    CPU::mmu = mmu;
    // Init values from Pandocs for DMG Gameboy
    CPU::AF.setWord(0x01B0);
    CPU::BC.setWord(0x0013);
    CPU::DE.setWord(0x00D8);
    CPU::HL.setWord(0x014D);
    CPU::SP.setWord(0xFFFE);
    CPU::PC.setWord(0x0100); // PC start location is 0x0100 for practical purposes

    mmu->writeByte(0xFF05, 0x00);
    mmu->writeByte(0xFF06, 0x00);
    mmu->writeByte(0xFF07, 0x00);
    mmu->writeByte(0xFF10, 0x80);
    mmu->writeByte(0xFF11, 0xBF);
    mmu->writeByte(0xFF12, 0xF3);
    mmu->writeByte(0xFF14, 0xBF);
    mmu->writeByte(0xFF16, 0x3F);
    mmu->writeByte(0xFF17, 0x00);
    mmu->writeByte(0xFF19, 0xBF);
    mmu->writeByte(0xFF1A, 0x7F);
    mmu->writeByte(0xFF1B, 0xFF);
    mmu->writeByte(0xFF1C, 0x9F);
    mmu->writeByte(0xFF1E, 0xBF);
    mmu->writeByte(0xFF20, 0xFF);
    mmu->writeByte(0xFF21, 0x00);
    mmu->writeByte(0xFF22, 0x00);
    mmu->writeByte(0xFF23, 0xBF);
    mmu->writeByte(0xFF24, 0x77);
    mmu->writeByte(0xFF25, 0xF3);
    mmu->writeByte(0xFF26, 0xF1);
    mmu->writeByte(0xFF40, 0x91);
    mmu->writeByte(0xFF42, 0x00);
    mmu->writeByte(0xFF43, 0x00);
    mmu->writeByte(0xFF45, 0x00);
    mmu->writeByte(0xFF47, 0xFC);
    mmu->writeByte(0xFF48, 0xFF);
    mmu->writeByte(0xFF49, 0xFF);
    mmu->writeByte(0xFF4A, 0x00);
    mmu->writeByte(0xFF4B, 0x00);
    mmu->writeByte(0xFFFF, 0x00);
}

CPU::~CPU() {}

// REGISTERS
Register CPU::getSP()
{
    return CPU::SP;
}
void CPU::setSP(u16 value)
{
    CPU::SP.setWord(value);
}
Register CPU::getPC()
{
    return CPU::PC;
}
void CPU::setPC(u16 value)
{
    CPU::PC.setWord(value);
    // printf("New PC Value: 0x%04X\n", CPU::PC.getWord());
}
Register CPU::getAF()
{
    return CPU::AF;
}
Register CPU::getBC()
{
    return CPU::BC;
}
Register CPU::getDE()
{
    return CPU::DE;
}
Register CPU::getHL()
{
    return CPU::HL;
}
void CPU::setAF(u16 value)
{
    CPU::AF.setWord(value);
}
void CPU::setBC(u16 value)
{
    CPU::BC.setWord(value);
}
void CPU::setDE(u16 value)
{
    CPU::DE.setWord(value);
}
void CPU::setHL(u16 value)
{
    CPU::HL.setWord(value);
}
void CPU::resetRegisters(){
    CPU::AF.setWord(0x00);
    CPU::BC.setWord(0x00);
    CPU::DE.setWord(0x00);
    CPU::HL.setWord(0x00);
    CPU::SP.setWord(0x00);
    CPU::PC.setWord(0x00);
}

bool CPU::getIME()
{
    return CPU::IME;
}
void CPU::setIME(bool value)
{
    CPU::IME = value;
}


void CPU::pushStackWord(u16 word) {
    // Push word to stack and decrement the pointer
    // Decrement push lower byte
    SP.setWord(SP.getWord() - 1);
    u8 lower = (u8) (word & 0x00FF);
    mmu->writeByte(SP.getWord(), lower);
    // Push higher byte
    SP.setWord(SP.getWord() - 1);
    u8 higher = (u8) ((word & 0xFF00) >> 8);
    mmu->writeByte(SP.getWord(), higher);
}

// Flag Operations

bool CPU::getZeroFlag()
{
    return CPU::AF.lower & ZERO_VALUE;
}
void CPU::setZeroFlag(bool set)
{
    if (set)
        CPU::AF.lower |= ZERO_VALUE;
    else
        CPU::AF.lower &= ~ZERO_VALUE;
}
bool CPU::getSubFlag()
{
    return CPU::AF.lower & SUB_VALUE;
}
void CPU::setSubFlag(bool set)
{
    if (set)
        CPU::AF.lower |= SUB_VALUE;
    else
        CPU::AF.lower &= ~SUB_VALUE;
}
bool CPU::getHCarryFlag()
{
    return CPU::AF.lower & HALF_VALUE;
}
void CPU::setHCarryFlag(bool set)
{
    if (set)
        CPU::AF.lower |= HALF_VALUE;
    else
        CPU::AF.lower &= ~HALF_VALUE;
}
bool CPU::getCarryFlag()
{
    return CPU::AF.lower & CARRY_VALUE;
}
void CPU::setCarryFlag(bool set)
{
    if (set)
        CPU::AF.lower |= CARRY_VALUE;
    else
        CPU::AF.lower &= ~CARRY_VALUE;
}

// Interrupts
void CPU::requestInterrupt(u8 interrupt)
{
    u8 IF = mmu->readByte(0xFF0F);
    IF |= interrupt;
    mmu->writeByte(0xFF0F, IF);
}

// Flag Helpers
bool CPU::checkHCarry_8(u8 arg1, u8 arg2)
{
    return ((arg1 & 0xF) + (arg2 & 0xF) > 0xF);
}
bool CPU::checkHCarry_16(u16 arg1, u16 arg2)
{
    return (((arg1 & 0xFFF) + (arg2 & 0xFFF)) > 0xFFF);
}
bool CPU::checkCarry_8(u8 arg1, u8 arg2)
{
    return (static_cast<int>(arg1) + static_cast<int>(arg2) > 0xFF);
}
bool CPU::checkCarry_16(u16 arg1, u16 arg2)
{
    return (static_cast<int>(arg1) + static_cast<int>(arg2) > 0xFFFF);
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
        timerCounter -= CPU_CLOCK_SPEED / frequency;
    }
}

u8 CPU::getInstruction()
{
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
    {
        // printf("NOP\n");
        return 1;
    }
    // STOP ? WTF
    case 0x10:
    {
        // printf("STOP\n");
        return 1;
    }
        return 1;
    // JR NZ i8?
    case 0x20:
        // printf("JR NZ ");
        if(!getZeroFlag())
        {
            signed char step = CPU::getInstruction();
            // printf("%d\n", step);
            PC.setWord(PC.getWord() + step);
            return 3;
        }
        // printf("\n");
        return 2;
    // JR NC i8?
    case 0x30:
        // printf("JR NC ");
        if(!getCarryFlag())
        {
            signed char step = CPU::getInstruction();
            // printf("%d\n", step);
            PC.setWord(PC.getWord() + step);
            return 3;
        } 
        return 2;
    // LD B, B
    case 0x40:
        // printf("LD B, %u\n", BC.higher);
        BC.higher = BC.higher;
        return 1;
    // LD D, B
    case 0x50:
        // printf("LD D, %u\n",BC.higher);
        DE.higher = BC.higher;
        return 1;
    // LD H, B
    case 0x60:
        // printf("LD H, %u\n", BC.higher);
        HL.higher = BC.higher;
        return 1;
    // LD (HL), B
    case 0x70:
        // printf("LD (HL), %u\n", BC.higher);
        mmu->writeByte(HL.getWord(), BC.higher);
        return 2;
    // LD BC,u16
    case 0x01:
        BC.lower = CPU::getInstruction();
        BC.higher = CPU::getInstruction();
        // printf("LD BC, %u\n", BC.getWord());
        return 3;
    // LD DE, u16
    case 0x11:
        DE.lower = CPU::getInstruction();
        DE.higher = CPU::getInstruction();
        // printf("LD DE, %u\n", DE.getWord());
        return 3;
    // LD HL, u16
    case 0x21:
        HL.lower = CPU::getInstruction();
        HL.higher = CPU::getInstruction();
        // printf("LD HL, %u\n", HL.getWord());
        return 3;
    // LD SP, u16
    case 0x31:
        SP.lower = CPU::getInstruction();
        SP.higher = CPU::getInstruction();
        // printf("LD SP, %u\n", SP.getWord());
        return 3;
    // LD B, C
    case 0x41:
        // printf("LD B, %u\n", BC.higher);
        BC.higher = BC.lower;
        return 1;
    // LD D, C
    case 0x51:
        // printf("LD D, %u\n", BC.higher);
        DE.higher = BC.lower;
        return 1;
    // LD H, C
    case 0x61:
        // printf("LD H, %u\n", BC.higher);
        HL.higher = BC.lower;
        return 1;
    // LD (HL), C
    case 0x71:
        // printf("LD (HL), %u\n", BC.higher);
        mmu->writeByte(HL.getWord(), BC.lower);
        return 2;
    // LD (BC),A
    case 0x02:
        // printf("LD (BC), %u\n", AF.higher);
        mmu->writeByte(HL.getWord(), AF.higher);
        return 2;
    // LD (DE),A
    case 0x12:
        // printf("LD (DE), %u\n", AF.higher);
        mmu->writeByte(DE.getWord(), AF.higher);
        return 2;
    // LD (HL+),A
    case 0x22:
        // printf("LD (HL+), %u\n", AF.higher);
        mmu->writeByte(HL.getWord(), AF.higher);
        HL.setWord(HL.getWord() + 1);
        return 2;
    // LD (HL-), A
    case 0x32:
        // printf("LD (HL-), %u\n", AF.higher);
        mmu->writeByte(HL.getWord(), AF.higher);
        HL.setWord(HL.getWord() - 1);
        return 2;
    // LD B, D
    case 0x42:
        // printf("LD B, %u\n", DE.lower);
        BC.higher = DE.higher;
        return 1;
    // LD D, D
    case 0x52:
        // printf("LD D, %u\n", DE.lower);
        DE.lower = DE.higher;
        return 1;
    // LD H, D
    case 0x62:
        // printf("LD H, %u\n", DE.lower);
        HL.higher = DE.higher;
        return 1;
    // LD (HL), D
    case 0x72:
        // printf("LD (HL), %u\n", DE.lower);
        mmu->writeByte(HL.getWord(), DE.higher);
        return 2;
    // INC BC ? should be 16 bit operation:
    case 0x03:
    {
        // printf("INC BC\n");
        CPU::inc_16(&BC);
        return 3;
    }
    // INC DE
    case 0x13:
    {
        // printf("INC DE\n");
        CPU::inc_16(&DE);
        return 3;
    }
    // INC HL
    case 0x23:
    {
        // printf("INC HL\n");
        CPU::inc_16(&HL);
        return 3;
    }
    // INC SP
    case 0x33:
    {
        // printf("INC SP\n");
        CPU::inc_16(&SP);
        return 3;
    }
    // LD B, E
    case 0x43:
        // printf("LD B, %u\n", DE.higher);
        BC.higher = DE.lower;
        return 1;
    // LD D, D
    case 0x53:
        // printf("LD D, %u\n", DE.higher);
        DE.higher = DE.higher;
        return 1;
    // LD H, D
    case 0x63:
        // printf("LD H, %u\n", DE.higher);
        HL.higher = DE.higher;
        return 1;
    // LD (HL), D
    case 0x73:
        // printf("LD (HL), %u\n", DE.higher);
        mmu->writeByte(HL.getWord(), DE.higher);
        return 2;
    
    // INC B
    case 0x04:
    {
        // printf("INC B\n");
        CPU::inc_8(&BC.higher);
        return 1;
    }
    // INC D
    case 0x14:
    {
        // printf("INC D\n");
        CPU::inc_8(&DE.lower);
        return 1;
    }
    // INC H
    case 0x24:
    {
        // printf("INC H\n");
        CPU::inc_8(&HL.higher);
        return 1;
    }
    // INC (HL)
    case 0x34:
    {
        // printf("INC (HL)\n");
        u8 byte = mmu->readByte(HL.getWord());
        u8 res = byte + 1;
        mmu->writeByte(HL.getWord(), res);
        CPU::setZeroFlag(!res);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(CPU::checkHCarry_16(HL.getWord(), 1));
        CPU::setCarryFlag(CPU::checkCarry_16(HL.getWord(), 1));
        return 2;
    }
    // LD B, H
    case 0x44:
        // printf("LD B, %u\n", HL.higher);
        BC.higher = HL.higher;
        return 1;
    // LD D, H
    case 0x54:
        // printf("LD D, %u\n", HL.higher);
        DE.higher = HL.higher;
        return 1;
    // LD H, H
    case 0x64:
        // printf("LD H, %u\n", HL.higher);
        HL.higher = HL.higher;
        return 1;
    // LD (HL), H
    case 0x74:
        // printf("LD (HL), %u\n", HL.higher);
        mmu->writeByte(HL.getWord(), HL.higher);
        return 2;
    // DEC B
    case 0x05:
    {
        // printf("DEC B\n");
        CPU::dec_8(&BC.higher);
        return 1;
    }
    // DEC D
    case 0x15:
    {
        // printf("DEC D\n");
        CPU::dec_8(&DE.higher);
        return 1;
    }
    // DEC H
    case 0x25:
    {
        // printf("DEC H\n");
        CPU::dec_8(&HL.higher);
        return 1;
    }
    // DEC (HL)
    case 0x35:
    {
        u8 byte = mmu->readByte(HL.getWord());
        u8 res = byte - 1;
        mmu->writeWord(HL.getWord(), res);
        CPU::setZeroFlag(!res);
        CPU::setSubFlag(1);
        CPU::setHCarryFlag(CPU::checkHCarry_8(byte, -1));
        CPU::setCarryFlag(CPU::checkCarry_8(byte, -1));
        return 1;
    }
    // LD B, L
    case 0x45:
        BC.higher = HL.lower;
        return 1;
    // LD D, L
    case 0x55:
        DE.lower = HL.lower;
        return 1;
    // LD H, L
    case 0x65:
        HL.higher = HL.lower;
        return 1;
    // LD (HL), L
    case 0x75:
        mmu->writeByte(HL.getWord(), HL.lower);
        return 2;
    // LD B,u8
    case 0x06:
        BC.higher = CPU::getInstruction();
        return 2;
    // LD D, u8
    case 0x16:
        DE.higher = CPU::getInstruction();
        return 2;
    // LD H, u8
    case 0x26:
        HL.higher = CPU::getInstruction();
        return 2;
    // LD (HL), u8
    case 0x36:
        mmu->writeByte(HL.getWord(), CPU::getInstruction());
        return 3;
    // LD B, (HL)
    case 0x46:
        BC.higher = mmu->readByte(HL.getWord());
        return 2;
    // LD D, (HL)
    case 0x56:
        DE.higher = mmu->readByte(HL.getWord());
        return 2;
    // LD H, (HL)
    case 0x66:
        HL.higher = mmu->readByte(HL.getWord());
        return 2;
    // HALT
    case 0x76:
        // printf("HALT (Incomplete)\n");
        return 1;
    // RLCA
    case 0x07:
        // printf("RLCA (Incomplete)\n");
        return 0;
    // RLA
    case 0x17:
        // printf("RLA (Incomplete)\n");
        return 0;
    // DAA
    case 0x27:
        // printf("DAA (Incomplete)\n");
        return 0;
    // SCF
    case 0x37:
        // printf("SCF (Incomplete)\n");
        return 0;
    // LD B, A
    case 0x47:
        // printf("LD B, %02X\n", AF.higher);
        BC.higher = AF.higher;
        return 1;
    // LD D, A
    case 0x57:
        DE.higher = AF.higher;
        return 1;
    // LD H, A
    case 0x67:
        HL.higher = AF.higher;
        return 1;
    // LD (HL), A
    case 0x77:
        mmu->writeByte(HL.getWord(), AF.higher);
        return 2;
    // LD (u16), SP
    case 0x08:
    {
        // printf("LD (u16), SP (Unsure)\n");
        u16 nn = mmu->readWord(PC.getWord() + 1);
        PC.setWord(PC.getWord() + 1);
        SP.setWord(nn);
        return 5;
    }
    // JR s8
    case 0x18:
    {
        signed char step = CPU::getInstruction();
        // printf("JR %d\n", step);
        PC.setWord(PC.getWord() + step);
        return 3;
    }
    // JR Z s8
    case 0x28:
    {
        // printf("JR Z ");
        if(getZeroFlag())
        {
            signed char step = CPU::getInstruction();
            // printf("%d\n", step);
            PC.setWord(PC.getWord() + step);
            return 3;
        }
        // printf("\n");
        return 2;
    }
    // JR C s8
    case 0x38:
    {
        // printf("JR C ");
        if(getCarryFlag())
        {
            signed char step = CPU::getInstruction();
            // printf("%d\n", step);
            PC.setWord(PC.getWord() + step);
            return 3;
        }
        // printf("\n");
        return 2;
    }
    // LD C, B
    case 0x48:
        BC.lower = BC.higher;
        return 1;
    // LD E, B
    case 0x58:
        DE.lower = BC.higher;
        return 1;
    // LD L, B
    case 0x68:
        HL.lower = BC.higher;
        return 1;
    // LD A, B
    case 0x78:
        AF.higher = BC.higher;
        return 1;
    // ADD HL,BC ? is machine cycle 2?
    case 0x09:
    {
        CPU::add_16(BC.getWord());
        return 2;
    }
    // ADD HL,DE
    case 0x19:
    {
        CPU::add_16(DE.getWord());
        return 2;
    }
    // ADD HL,HL
    case 0x29:
    {
        CPU::add_16(HL.getWord());
        return 2;
    }
    // ADD HL,SP
    case 0x39:
    {
        CPU::add_16(SP.getWord());
        return 2;
    }
    // LD C, C
    case 0x49:
        BC.lower = BC.lower;
        return 1;
    // LD E, C
    case 0x59:
        DE.lower = BC.lower;
        return 1;
    // LD L, C
    case 0x69:
        HL.lower = BC.lower;
        return 1;
    // LD A, C
    case 0x79:
        AF.higher = BC.lower;
        return 1;
    // LD A, (BC)
    case 0x0A:
        AF.higher = mmu->readByte(BC.getWord());
        return 2;
    // LD A, (DE)
    case 0x1A:
        AF.higher = mmu->readByte(DE.getWord());
        return 2;
    // LD A, (HL+)
    case 0x2A:
        // printf("LD A, 0x%02hX from 0x%04hX\n", mmu->readByte(HL.getWord()), HL.getWord());
        AF.higher = mmu->readByte(HL.getWord());
        HL.setWord(HL.getWord() + 1);
        return 2;
    // LD A, (HL-)
    case 0x3A:
        AF.higher = mmu->readByte(HL.getWord());
        HL.setWord(HL.getWord() - 1);
        return 2;
    // LD C, D
    case 0x4A:
        BC.lower = DE.higher;
        return 1;
    // LD E, D
    case 0x5A:
        DE.lower = DE.higher;
        return 1;
    // LD L, D
    case 0x6A:
        HL.lower = DE.higher;
        return 1;
    // LD A, D
    case 0x7A:
        AF.higher = DE.higher;
        return 1;
    // DEC BC
    case 0x0B:
    {
        CPU::dec_16(&BC);
        return 3;
    }
    // DEC DE
    case 0x1B:
    {
        CPU::dec_16(&DE);
        return 3;
    }
    // DEC HL
    case 0x2B:
    {
        CPU::dec_16(&HL);
        return 3;
    }
    // DEC SP
    case 0x3B:
    {
        CPU::dec_16(&SP);
        return 3;
    }
    // LD C, E
    case 0x4B:
        BC.lower = DE.lower;
        return 1;
    // LD E, E
    case 0x5B:
        DE.lower = DE.lower;
        return 1;
    // LD L, E
    case 0x6B:
        HL.lower = DE.lower;
        return 1;
    // LD A, D
    case 0x7B:
        AF.higher = DE.higher;
        return 1;
    // INC C
    case 0x0C:
    {
        CPU::inc_8(&BC.lower);
        return 1;
    }
    // INC E
    case 0x1C:
    {
        CPU::inc_8(&DE.lower);
        return 1;
    }
    // INC L
    case 0x2C:
    {
        CPU::inc_8(&HL.lower);
        return 1;
    }
    // INC A
    case 0x3C:
    {
        CPU::inc_8(&AF.higher);
        return 1;
    }
    // LD C, H
    case 0x4C:
        BC.lower = HL.higher;
        return 1;
    // LD E, H
    case 0x5C:
        DE.lower = HL.higher;
        return 1;
    // LD L, H
    case 0x6C:
        HL.lower = HL.higher;
        return 1;
    // LD A, H
    case 0x7C:
        AF.higher = HL.higher;
        return 1;
    // DEC C
    case 0x0D:
    {
        CPU::dec_8(&BC.lower);
        return 1;
    }
    // DEC E
    case 0x1D:
    {
        CPU::dec_8(&DE.lower);
        return 1;
    }
    // DEC L
    case 0x2D:
    {
        CPU::dec_8(&HL.lower);
        return 1;
    }
    // DEC A
    case 0x3D:
    {
        CPU::dec_8(&AF.higher);
        return 1;
    }
    // LD C, L
    case 0x4D:
        BC.lower = HL.lower;
        return 1;
    // LD E, L
    case 0x5D:
        DE.lower = HL.lower;
        return 1;
    // LD L, L
    case 0x6D:
        HL.lower = HL.lower;
        return 1;
    // LD A, L
    case 0x7D:
        AF.higher = HL.lower;
        return 1;
    // LD C, u8
    case 0x0E:
        BC.lower = CPU::getInstruction();
        return 2;
    // LD E, u8
    case 0x1E:
        DE.lower = CPU::getInstruction();
        return 2;
    // LD L, u8
    case 0x2E:
        HL.lower = CPU::getInstruction();
        return 2;
    // LD A, u8
    case 0x3E:
        AF.higher = CPU::getInstruction();
        return 2;
    // LD C, (HL)
    case 0x4E:
        BC.lower = mmu->readByte(HL.getWord());
        return 1;
    // LD E, (HL)
    case 0x5E:
        DE.lower = mmu->readByte(HL.getWord());
        return 1;
    // LD L, (HL)
    case 0x6E:
        HL.lower = mmu->readByte(HL.getWord());
        return 1;
    // LD A, (HL)
    case 0x7E:
        AF.higher = mmu->readByte(HL.getWord());
        return 1;
    // LD C, A
    case 0x4F:
        BC.lower = AF.higher;
        return 1;
    // LD E, A
    case 0x5F:
        DE.lower = AF.higher;
        return 1;
    // LD L, A
    case 0x6F:
        HL.lower = AF.higher;
        return 1;
    // LD A, A
    case 0x7F:
        AF.higher = AF.higher;
        return 1;
    // ADD A, B
    case 0x80:
    {
        CPU::add_8(BC.higher);
        return 1;
    }
    // ADD A, C
    case 0x81:
    {
        CPU::add_8(BC.lower);
        return 1;
    }
    // ADD A, D
    case 0x82:
    {
        CPU::add_8(DE.higher);
        return 1;
    }
    // ADD A, E
    case 0x83:
    {
        CPU::add_8(DE.lower);
        return 1;
    }
    // ADD A, H
    case 0x84:
    {
        CPU::add_8(HL.higher);
        return 1;
    }
    // ADD A, L
    case 0x85:
    {
        CPU::add_8(HL.lower);
        return 1;
    }
    // ADD A, (HL)
    // Adds to the 8-bit A register,
    // data from the absolute address specified by the 16-bit register HL,
    // stores result back into the A register.
    case 0x86:
    {
        int result, carry_bit = AF.higher + mmu->readByte(HL.getWord());
        CPU::add_8(mmu->readByte(HL.getWord()));
        return 1;
    }
    // ADD A, A
    case 0x87:
    {
        CPU::add_8(AF.higher);
        return 1;
    }
    // ADC A, B
    case 0x88:
    {
        CPU::add_8(BC.higher + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, C
    case 0x89:
    {
        CPU::add_8(BC.lower + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, D
    case 0x8A:
    {
        CPU::add_8(DE.higher + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, E
    case 0x8B:
    {
        CPU::add_8(DE.lower + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, H
    case 0x8C:
    {
        CPU::add_8(HL.higher + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, L
    case 0x8D:
    {
        CPU::add_8(HL.lower + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, (HL)
    // Adds to the 8-bit A register,
    // the carry flag and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x8E:
    {
        CPU::add_8(mmu->readByte(HL.getWord()) + CPU::getCarryFlag());
        return 1;
    }
    // ADC A, A
    case 0x8F:
    {
        CPU::add_8(AF.higher + CPU::getCarryFlag());
        return 1;
    }
    // SUB A, B
    case 0x90:
    {
        CPU::sub_a(BC.higher);
        return 1;
    }
    // SUB A, C
    case 0x91:
    {
        CPU::sub_a(BC.lower);
        return 1;
    }
    // SUB A, D
    case 0x92:
    {
        CPU::sub_a(DE.higher);
        return 1;
    }
    // SUB A, E
    case 0x93:
    {
        CPU::sub_a(DE.lower);
        return 1;
    }
    // SUB A, H
    case 0x94:
    {
        CPU::sub_a(HL.higher);
        return 1;
    }
    // SUB A, L
    case 0x95:
    {
        CPU::sub_a(HL.lower);
        return 1;
    }
    // SUB A, (HL)
    // Subtracts from the 8-bit A register,
    // data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x96:
    {
        CPU::sub_a(mmu->readByte(HL.getWord()));
        return 2;
    }
    // SUB A, A
    case 0x97:
    {
        CPU::sub_a(AF.higher);
        return 1;
    }
    // SBC A, B
    case 0x98:
    {
        CPU::sub_a(BC.higher - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, C
    case 0x99:
    {
        CPU::sub_a(BC.lower - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, D
    case 0x9A:
    {
        CPU::sub_a(DE.higher - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, E
    case 0x9B:
    {
        CPU::sub_a(DE.lower - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, H
    case 0x9C:
    {
        CPU::sub_a(HL.higher - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, L
    case 0x9D:
    {
        CPU::sub_a(HL.lower - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, (HL)
    // Subtracts from the 8-bit A register,
    // the carry flag and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0x9E:
    {
        CPU::sub_a(mmu->readByte(HL.getWord()) - CPU::getCarryFlag());
        return 1;
    }
    // SBC A, A
    case 0x9F:
    {
        CPU::sub_a(AF.higher - CPU::getCarryFlag());
        return 1;
    }
    // AND A, B: Bitwise AND
    case 0xA0:
        CPU::and_a(BC.higher);
        return 1;
    // AND A, C
    case 0xA1:
        CPU::and_a(BC.lower);
        return 1;
    // AND A, D
    case 0xA2:
        CPU::and_a(DE.higher);
        return 1;
    // AND A, E
    case 0xA3:
        CPU::and_a(DE.lower);
        return 1;
    // AND A, H
    case 0xA4:
        CPU::and_a(HL.higher);
        return 1;
    // AND A, L
    case 0xA5:
        CPU::and_a(HL.lower);
        return 1;
    // AND A, (HL)
    case 0xA6:
        CPU::and_a(mmu->readByte(HL.getWord()));
        return 2;
    // AND A, A
    case 0xA7:
        CPU::and_a(AF.higher);
        return 1;
    // XOR A, B
    case 0xA8:
        CPU::xor_a(BC.higher);
        return 1;
    // XOR A, C
    case 0xA9:
        CPU::xor_a(BC.lower);
        return 1;
    // XOR A, D
    case 0xAA:
        CPU::xor_a(DE.higher);
        return 1;
    // XOR A, E
    case 0xAB:
        CPU::xor_a(DE.lower);
        ;
        return 1;
    // XOR A, H
    case 0xAC:
        CPU::xor_a(HL.higher);
        return 1;
    // XOR A, L
    case 0xAD:
        CPU::xor_a(HL.lower);
        return 1;
    // XOR (HL)
    // Performs a bitwise XOR operation between
    // the 8-bit A register and data from the absolute address specified by the 16-bit register HL,
    // and stores the result back into the A register.
    case 0xAE:
        CPU::xor_a(mmu->readByte(HL.getWord()));
        return 2;
    // XOR A,A
    case 0xAF:
        // printf("XOR A, A\n");
        CPU::xor_a(AF.higher);
        return 1;
    // OR A, B
    case 0xB0:
        CPU::or_a(BC.higher);
        return 1;
    // OR A, C
    case 0xB1:
        CPU::or_a(BC.lower);
        return 1;
    // OR A, D
    case 0xB2:
        CPU::or_a(DE.higher);
        return 1;
    // OR A, E
    case 0xB3:
        CPU::or_a(DE.lower);
        return 1;
    // OR A, H
    case 0xB4:
        CPU::or_a(HL.higher);
        return 1;
    // OR A, L
    case 0xB5:
        CPU::or_a(HL.lower);
        return 1;
    // OR A, (HL)
    case 0xB6:
        CPU::or_a(mmu->readByte(HL.getWord()));
        return 2;
    // OR A, A
    case 0xB7:
        CPU::or_a(AF.higher);
        return 1;
    // CP A,B
    case 0xB8:
    {
        CPU::cp(BC.higher);
        return 1;
    }
    // CP A,C
    case 0xB9:
    {
        CPU::cp(BC.lower);
        return 1;
    }
    // CP A,D
    case 0xBA:
    {
        CPU::cp(DE.higher);
        return 1;
    }
    // CP A,E
    case 0xBB:
    {
        CPU::cp(DE.lower);
        return 1;
    }
    // CP A,H
    case 0xBC:
    {
        CPU::cp(HL.higher);
        return 1;
    }
    // CP A,L
    case 0xBD:
    {
        CPU::cp(HL.lower);
        return 1;
    }
    // CP A,(HL)
    case 0xBE:
    {
        CPU::cp(mmu->readByte(HL.getWord()));
        return 2;
    }
    // CP A,A
    case 0xBF:
    {
        CPU::cp(AF.higher);
        return 1;
    }
    // RET NZ
    case 0xC0:
    {
        if (!CPU::getZeroFlag())
        {
            CPU::ret();
            return 5;
        }
        else
            return 2;
    }
    // POP BC
    case 0xC1:
        CPU::pop(&BC);
        return 3;
    // JP NZ, u16 -- REVIEW
    case 0xC2:
    {
        if (!CPU::getZeroFlag())
        {
            CPU::jp();
            return 4;
        }
        else
            return 3;
    }
    // JP u16:
    case 0xC3:
    {
        CPU::jp();
        return 4;
    }
    // CALL NZ, u16
    case 0xC4:
    {
        if (CPU::getZeroFlag() == 0)
        {
            return 6;
        }
        else
            return 3;
    }
    // PUSH BC
    case 0xC5:
        return 4;
    // ADD A, d8 -- REVIEW
    case 0xC6: {
        u8 byte = CPU::getInstruction();
        CPU::add_8(byte);
        // AF.higher += d8; // don't know how to get d8 value
        return 2;
        }
    // RST 0
    case 0xC7:
        return 4;
    // RET Z
    case 0xC8:
        if (CPU::getZeroFlag())
        {
            CPU::ret();
            return 5;
        }
        return 2;
    // RET
    case 0xC9:
    {
        CPU::ret();
        return 4;
    }
    // JP Z, a16
    case 0xCA:
    {
        if (CPU::getZeroFlag())
        {
            CPU::jp();
            return 4;
        }
        else
        {
            return 3;
        }
    }
    // CALL Z, a16
    case 0xCC:
    {
        if (CPU::getZeroFlag())
        {
            CPU::call();
            return 6;
        }
        else
        {
            return 3;
        }
    }
    // CALL a16
    case 0xCD:
    {
        CPU::call();
        return 6;
    }
    // ADC A, d8
    case 0xCE:
    {
        u8 byte = CPU::getInstruction();
        CPU::add_8(byte + CPU::getCarryFlag());
        return 2;
    }
    // RST 1
    case 0xCF:
    {
        // TODO
        return 4;
    }
    // RET NC
    case 0xD0:
    {
        if (!CPU::getCarryFlag())
        {
            CPU::ret();
            return 5;
        }
        else
        {
            return 2;
        }
    }
    // POP DE
    case 0xD1:
    {
        CPU::pop(&DE);
        return 3;
    }
    // JP NC, a16
    case 0xD2:
    {
        if (!CPU::getCarryFlag())
        {
            CPU::jp();
            return 4;
        }
        else
        {
            return 3;
        }
    }
    // CALL NC, a16
    case 0xD4:
    {
        if (!CPU::getCarryFlag())
        {
            return 6;
        }
        else
        {
            return 3;
        }
    }
    // PUSH DE
    case 0xD5:
    {
        SP.setWord(SP.getWord() - 1);
        DE.higher = mmu->readByte(SP.getWord() - 1);
        SP.setWord(SP.getWord() - 2);
        DE.lower = mmu->readByte(SP.getWord());
        SP.setWord(SP.getWord() - 2);
        return 4;
    }
    // SUB d8
    case 0xD6:
    {
        u8 byte = CPU::getInstruction();
        CPU::sub_a(byte);
        return 2;
    }
    // RST 2
    case 0xD7:
    {
        return 4;
    }
    // RET C
    case 0xD8:
    {
        if (CPU::getCarryFlag())
        {
            CPU::ret();
            return 5;
        }
        else
        {
            return 0;
        }
    }
    case 0xD9: // RETI
    {
        PC.setWord(mmu->readWord(SP.getWord()));
        SP.setWord(SP.getWord() + 2);
        CPU::setIME(true);
        return 4;
    }
    // JP C, a16
    case 0xDA:
    {
        if (CPU::getCarryFlag())
        {
            CPU::jp();
            return 4;
        }
        else
        {
            return 3;
        }
    }
    // CALL C, u16
    case 0xDC:
    {
        if (CPU::getCarryFlag() == 1){
            CPU::call();
            return 6;
        } else {
            return 3;
        }
    }
    // SBC A, d8 -- REVIEW
    case 0xDE:
    {
        u8 byte = CPU::getInstruction();
        CPU::sub_a(byte + CPU::getCarryFlag());
        return 2;
    }
    // RST 3
    case 0xDF:
    {
        return 4;
    }
    // LD (a8), A -- REVIEW
    case 0xE0:
    {
        u8 byte = CPU::getInstruction();
        u16 addr = 0xFF00 + byte;
        mmu->writeByte(addr, AF.higher);
        return 3;  
    }
    // POP HL
    case 0xE1:
    {
        CPU::pop(&HL);
        return 3;
    }
    // LD (C), A
    case 0xE2:
    {
        // confused
        return 2;
    }
    // PUSH HL
    case 0xE5:
    {
        HL.setWord(HL.getWord() - 1);
        DE.higher = mmu->readByte(HL.getWord() - 1);
        HL.setWord(HL.getWord() - 2);
        DE.lower = mmu->readByte(HL.getWord());
        HL.setWord(HL.getWord() - 2);
        return 4;
    }
    // AND d8
    case 0xE6:
    {
        u8 byte = CPU::getInstruction();
        CPU::and_a(byte);
        return 2;
    }
    // RST 4
    case 0xE7:
    {
        return 4;
    }
    // ADD SP, s8
    case 0xE8:
    {
        signed char byte = CPU::getInstruction();
        CPU::add_8(byte);
        return 4;
    }
    // JP HL
    case 0xE9:
    {
        CPU::jp_hl();
        return 1;
    }
    // LD (a16), A
    case 0xEA:
    {
        u8 lower = CPU::getInstruction();
        u8 higher = CPU::getInstruction();
        u16 addr = (higher << 8) | lower;
        mmu->writeByte(addr, AF.higher);
        // Store the contents of register A in the internal RAM or register specified by the 16-bit immediate operand a16.
        return 4;
    }
    // XOR d8
    case 0xEE:
    {
        u8 byte = CPU::getInstruction();
        CPU::xor_a(byte);
        return 2;
    }
    // RST 5
    case 0xEF:
    {
        return 4;
    }
    // LD A, (a8)
    case 0xF0:
    {
        u8 byte = CPU::getInstruction();
        u16 addr = 0xFF00 + byte;
        AF.higher = mmu->readByte(addr);
        return 3;
    }
    // POP AF
    case 0xF1:
    {
        CPU::pop(&AF);
        return 3;
    }
    // LD A, (C)
    case 0xF2:
    {
        u16 addr = 0xFF00 + BC.lower;
        AF.higher = mmu->readByte(addr);
        return 1;
    }
    // DI
    case 0xF3:
    {
        CPU::setIME(false);
        return 1;
    }
    // PUSH AF
    case 0xF5:
    {
        SP.setWord(SP.getWord() - 1);
        AF.higher = mmu->readByte(SP.getWord() - 1);
        SP.setWord(SP.getWord() - 2);
        AF.higher = mmu->readByte(SP.getWord());
        SP.setWord(SP.getWord() - 2);
        return 4;
    }
    // OR d8
    case 0xF6:
    {
        u8 byte = CPU::getInstruction();
        CPU::or_a(byte);
        return 2;
    }
    // RST 6
    case 0xF7:
    {
        return 4;
    }
    // LD HL, SP+s8
    case 0xF8:
    {
        // Add the 8-bit signed operand s8 (values -128 to +127) to the stack pointer SP, 
        // and store the result in register pair HL.
        signed char byte = CPU::getInstruction();
        u16 res = SP.getWord() + byte;
        HL.setWord(res); // what to replace for s8?
        CPU::setZeroFlag(!res);
        CPU::setSubFlag(0);
        CPU::setHCarryFlag(CPU::checkHCarry_16(SP.getWord(), byte));
        CPU::setCarryFlag(CPU::checkCarry_16(SP.getWord(), byte));
        return 3;
    }
    // LD SP, HL
    case 0xF9:
    {
        SP.setWord(HL.getWord());
        return 2;
    }
    // LD A, (a16) -- REVIEW
    case 0xFA:
    {
        // Load into register A the contents of the internal RAM or 
        // register specified by the 16-bit immediate operand a16.
        u8 lower = CPU::getInstruction();
        u8 higher = CPU::getInstruction();
        u16 addr = (higher << 8) | lower;
        AF.higher = mmu->readByte(addr);
        return 4;
    }
    // EI -- REVIEW
    case 0xFB:
    {
        CPU::setIME(true);
        return 1;
    }
    // CP d8
    case 0xFE:
    {
        u8 byte = CPU::getInstruction();
        CPU::cp(byte);
        return 2;
    }
    // RST 7
    case 0xFF:
    {
        // confused
        return 4;
    }
    default:
        // printf("unkown opcode %02X\n", instruction);
        return 1;
    }
}

void CPU::add_8(u8 arg)
{
    u16 res = AF.higher + arg;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(false);
    CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, arg));
    CPU::setCarryFlag(CPU::checkCarry_8(AF.higher, arg));
    AF.higher = res;
}

void CPU::add_16(u16 arg)
{
    u16 res = AF.getWord() + arg;
    CPU::setZeroFlag(res == 0);
    CPU::setSubFlag(false);
    CPU::setHCarryFlag(CPU::checkHCarry_16(AF.getWord(), arg));
    CPU::setCarryFlag(CPU::checkCarry_16(AF.getWord(), arg));
    AF.setWord(res);
}

void CPU::sub_a(u8 arg)
{
    u16 res = AF.higher - arg;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(true);
    CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, arg));
    CPU::setCarryFlag(CPU::checkCarry_8(AF.higher, arg));
    AF.higher = res;
}

void CPU::or_a(u8 arg)
{
    AF.higher |= arg;
    CPU::setZeroFlag(AF.higher == 0);
    CPU::setSubFlag(0);
    CPU::setHCarryFlag(0);
    CPU::setCarryFlag(0);
}

void CPU::and_a(u8 arg)
{
    AF.higher &= arg;
    CPU::setZeroFlag(AF.higher == 0);
    CPU::setSubFlag(0);
    CPU::setHCarryFlag(1);
    CPU::setCarryFlag(0);
}

void CPU::xor_a(u8 arg)
{
    u8 res = AF.higher ^ arg;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(false);
    CPU::setHCarryFlag(false);
    CPU::setCarryFlag(false);
    AF.higher = res;
}

void CPU::cp(u8 arg)
{
    u16 res = AF.higher - arg;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(true);
    CPU::setHCarryFlag(CPU::checkHCarry_8(AF.higher, arg));
    CPU::setCarryFlag(CPU::checkCarry_8(AF.higher, arg));
}

void CPU::pop(Register *reg)
{
    reg->lower = mmu->readByte(SP.getWord());
    SP.setWord(SP.getWord() + 1);
    reg->higher = mmu->readByte(SP.getWord());
    SP.setWord(SP.getWord() + 1);
}

void CPU::jp()
{
    u8 lower = CPU::getInstruction();
    u8 higher = CPU::getInstruction();
    PC.lower = lower;
    PC.higher = higher;
}

void CPU::jp_hl()
{
    PC.lower = HL.lower;
    PC.higher = HL.higher;
}

void CPU::ret()
{
    PC.lower = mmu->readByte(SP.getWord());
    SP.setWord(SP.getWord() + 1);
    PC.higher = mmu->readByte(SP.getWord());
    SP.setWord(SP.getWord() + 1);
}

void CPU::call()
{
    // printf("CALL\n");
    u8 lower = CPU::getInstruction();
    u8 higher = CPU::getInstruction();
    SP.setWord(SP.getWord() - 1);
    mmu->writeByte(SP.getWord(), PC.higher);
    SP.setWord(SP.getWord() - 1);
    mmu->writeByte(SP.getWord(), PC.lower);
    PC.lower = lower;
    PC.higher = higher;
    // printf("PC: %02X\n", PC.getWord());
}

void CPU::inc_8(u8 *reg)
{
    u8 byte = *reg;
    u8 res = byte + 1;
    *reg = res;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(0);
    CPU::setHCarryFlag(CPU::checkHCarry_8(byte, 1));
}

void CPU::inc_16(Register *reg)
{
    u16 word = reg->getWord();
    u16 res = word + 1;
    reg->setWord(res);
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(0);
    CPU::setHCarryFlag(CPU::checkHCarry_16(word, 1));
}

void CPU::dec_8(u8 *reg)
{
    u8 byte = *reg;
    u8 res = byte - 1;
    *reg = res;
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(1);
    CPU::setHCarryFlag(CPU::checkHCarry_8(byte, -1));
    CPU::setCarryFlag(CPU::checkCarry_8(byte, -1));
}

void CPU::dec_16(Register *reg)
{
    u16 word = reg->getWord();
    u16 res = word - 1;
    reg->setWord(res);
    CPU::setZeroFlag(!res);
    CPU::setSubFlag(1);
    CPU::setHCarryFlag(CPU::checkHCarry_16(word, -1));
    CPU::setCarryFlag(CPU::checkCarry_16(word, -1));
}

// DEBUG

void CPU::dumpRegisters()
{
    printf("AF: 0x%04hX z: %s n: %s h: %s c: %s\n", AF.getWord(), CPU::getZeroFlag() ? "true" : "false", CPU::getSubFlag() ? "true" : "false", CPU::getHCarryFlag() ? "true" : "false", CPU::getCarryFlag() ? "true" : "false");
    printf("BC: 0x%04hX\n", BC.getWord());
    printf("DE: 0x%04hX\n", DE.getWord());
    printf("HL: 0x%04hX\n", HL.getWord());
    printf("SP: 0x%04hX\n", SP.getWord());
    printf("PC: 0x%04hX\n", PC.getWord());
    // std::cout << "AF: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::AF.getWord() << " (" << std::bitset<16>(CPU::AF.getWord()) << ")\n";
    // std::cout << "BC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::BC.getWord() << " (" << std::bitset<16>(CPU::BC.getWord()) << ")\n";
    // std::cout << "DE: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::DE.getWord() << " (" << std::bitset<16>(CPU::DE.getWord()) << ")\n";
    // std::cout << "HL: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::HL.getWord() << " (" << std::bitset<16>(CPU::HL.getWord()) << ")\n";
    // std::cout << "SP: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::SP.getWord() << " (" << std::bitset<16>(CPU::SP.getWord()) << ")\n";
    // std::cout << "PC: 0x" << std::hex << std::setw(4) << std::setfill('0') << +CPU::PC.getWord() << " (" << std::bitset<16>(CPU::PC.getWord()) << ")\n";
    // printf("zero: %s\n", CPU::getZeroFlag() ? "true" : "false");

    // printf("IME: %s\n", CPU::getIME() ? "true" : "false");
    // printf("IF: %04X\n", mmu->readByte(0xFF0F)); // Interrupt Flag Register
    // printf("IE: %04X\n", mmu->readByte(0xFFFF));
}