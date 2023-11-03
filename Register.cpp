#include <iostream>

#include "Register.h"

/**
 * @brief A register struct. Contains an 8-bit lower and upper register, 
 * which can also be accessed and written to in 16-bit paired form.
 */
struct Register
{
    u8 lower;   ///< Lower 8-bits of the register pair.
    u8 higher;  ///< Higher 8-bits of the register pair.

    /**
     * @brief Get the combined values of a register pair.
     * 
     * @return The full 16-bit register pair.
     */
    u16 getWord() {
        return (u16) ((higher << 8) | lower); // upper
    }

    /**
     * @brief Set the value of a register pair.
     * 
     * @param word A 16-bit value to assign to a register pair.
     * The the higher and lower 8-bits are masked and shifted 
     * appropriately before being assigned.
     */
    void setWord(u16 word) {
        higher = (u8) ((word & 0xFF00) >> 8); // mask to first 4 bits, shift right 8 bits.
        lower = (u8) (word & 0x00FF); // mask to only last 4 bits

        // Debug
        std::cout << "Word: " << word << " Higher: " << +higher << " Lower: " << +lower << std::endl;
    }
};
