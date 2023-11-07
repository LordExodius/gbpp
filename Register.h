#ifndef REGISTER_H_INCLUDED
#define REGISTER_H_INCLUDED
#include "global.h"

/**
 * @brief A register struct. Contains an 8-bit lower and upper register, 
 * which can also be accessed and written to in 16-bit paired form.
 */
struct Register
{
    /*
    Note: Because the Gameboy's architecture is little-endian, the least significant bytes come first.
    I.e., given a 16-bit value, the bits would be in the following order:

    Lower: 7 6 5 4 3 2 1 0
    Higher: 15 14 13 11 10 9 8
    */

    u8 lower;           ///< Lower 8-bits of the register pair.
    u8 higher;          ///< Higher 8-bits of the register pair.

    /**
     * @brief Get the combined values of a register pair.
     * 
     * @return The full 16-bit register pair.
     */
    u16 getWord();

    /**
     * @brief Assigns the value of a register pair.
     * 
     * @param word A 16-bit value to assign to a register pair.
     */
    void setWord(u16);
};

#endif