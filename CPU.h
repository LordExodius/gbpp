#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "Register.h"
#include "MMU.h"

#define CPU_CLOCK_SPEED 4194304;

#define ZERO_VALUE 0x80
#define SUB_VALUE 0x40
#define HALF_VALUE 0x20
#define CARRY_VALUE 0x10

#define TIMA 0xFF05
#define TMA 0xFF06
#define TMC 0xFF07

#define TMA_0 = 4096
#define TMA_1 = 262144
#define TMA_2 = 65536
#define TMA_3 = 16384

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

    MMU *mmu;

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
    void updateTimer(int cycles);

    // DEBUG
    void dumpRegisters();
};
#endif