#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;

/**
 * @brief A register struct. Contains an 8-bit lower and upper register, 
 * which can also be accessed and written to in 16-bit paired form.
 */
struct Register
{
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