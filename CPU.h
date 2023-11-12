#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "global.h"
#include "Register.h"
#include "MMU.h"

#define CPU_CLOCK_SPEED 4194304

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
    Register BC;    ///< `B` and `C` register pair ().
    Register DE;    ///< `D` and `E` register pair ().
    Register HL;    ///< `H` and `L` register pair.
    Register PC;    ///< Program Counter.
    Register SP;    ///< Stack Pointer.

    // Memory
    MMU *mmu;

    // Timer
    int divCounter = 0;
    int timerCounter = 0;

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