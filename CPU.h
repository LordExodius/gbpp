#include "Register.h"
#include "MMU.h"

#define ZERO_VALUE 0x8
#define SUB_VALUE 0x4
#define HALF_VALUE 0x2
#define CARRY_VALUE 0x1

class CPU
{
private:
    // Registers
    static Register AF;    ///< `A` and `F` register pair (Accumulator and Flag registers).
    static Register BC;    ///< `B` and `C` register pair ().
    static Register DE;    ///< `D` and `E` register pair ().
    static Register HL;    ///< `H` and `L` register pair.
    static Register PC;    ///< Program Counter.
    static Register SP;    ///< Stack Pointer.

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

    u8 fetchOp(u16);
    void decodeOp(u8);

    // Debug
    void resetRegisters();
    void printRegisters();
};
