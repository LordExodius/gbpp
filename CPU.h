#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "global.h"
#include "Register.h"
#include "MMU.h"

#define ZERO_VALUE 0x80
#define SUB_VALUE 0x40
#define HALF_VALUE 0x20
#define CARRY_VALUE 0x10

#define DIV_ADDR 0xFF04
#define TIMA_ADDR 0xFF05
#define TMA_ADDR 0xFF06
#define TAC_ADDR 0xFF07

#define DIV_SPEED 16384
#define TAC_0 4096
#define TAC_1 262144
#define TAC_2 65536
#define TAC_3 16384

class CPU
{
private:
    // Registers
    Register AF;    ///< `A` and `F` register pair (Accumulator and Flag registers).
    Register BC;    ///< `B` and `C` register pair.
    Register DE;    ///< `D` and `E` register pair.
    Register HL;    ///< `H` and `L` register pair.
    Register PC;    ///< Program Counter.
    Register SP;    ///< Stack Pointer.

    // Memory
    MMU *mmu;       ///< Pointer to MMU object associated with the emulator.

    // Timer
    int divCounter = 0;     ///< Internal counter used to determine the number of CPU cycles passed before incrementing DIV
    int timerCounter = 0;   ///< Internal counter used to determine the number of CPU cycles passed before incrementing TIMA

public:
    /**
     * @brief Construct a new `CPU` object.
     * 
     * @param mmu Pointer to MMU instance in `Emulator` class.
     */
    CPU(MMU *mmu);

    /**
     * @brief Destroy the `CPU::CPU` object
     */
    ~CPU();

    // FLAGS
    bool getZeroFlag();
    void setZeroFlag(bool);
    bool getSubFlag();
    void setSubFlag(bool);
    bool getHCarryFlag();
    void setHCarryFlag(bool);
    bool getCarryFlag();
    void setCarryFlag(bool);

    // Helpers
    bool checkHCarry_8(u8 arg1, u8 arg2, u8 res);
    bool checkHCarry_16(u16 arg1, u16 arg2, u16 res);
    bool checkCarry_8(u8 arg1, u8 arg2);
    bool checkCarry_16(u16 arg1, u16 arg2);

    // Instructions
    u8 getInstruction();
    /**
     * @brief Given an 8-bit CPU instruction, execute the associated Opcode and update flags as necessary.
     * 
     * @param instruction An 8-bit encoded CPU opcode.
     * @return `int` The number of M-cycles taken to execute the opcode.
     */
    int executeInstruction(u8 instruction);

    void add_a(u8 arg);
    void sub_a(u8 arg);
    void add_hl(u16 arg);
    void add_sp(s8 arg);
    void adc();
    void xor_a(u8 arg);
    void cp(u8 arg);
    void pop(Register *reg);
    void jp();
    void jp_hl();
    void ret();
    void call();


    // Timer
    u8 getDivider();
    u8 getTimer();
    u8 getTimerModulo();
    void resetDivider();
    void setDivider(u8 value);
    void setTimer(u8 value);
    void setTimerModulo(u8 value);
    void updateTimer(int cycles);

    // DEBUG
    void dumpRegisters();
};
#endif