#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED
#include "Register.h"

#define ZERO_VALUE 0x80
#define SUB_VALUE 0x40
#define HALF_VALUE 0x20
#define CARRY_VALUE 0x10

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

public:
    /**
     * @brief Constructor for CPU object. Initializes register and timer values appropriately.
     */
    CPU(/* args */);

    /**
     * @brief Destroy the CPU::CPU object
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

    // DEBUG
    void dumpRegisters();
};
#endif