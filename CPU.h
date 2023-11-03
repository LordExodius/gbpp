#include "Register.h"

class CPU
{
private:
    // Registers
    Register AF;    ///< `A` and `F` register pair (Accumulator and Flag registers).
    Register BC;    ///< `B` and `C` register pair  .
    Register DE;    ///< `D` and `E` register pair.
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

    // Debug
    void resetRegisters();
    void printRegisters();
};
