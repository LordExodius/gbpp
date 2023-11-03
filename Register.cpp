#include "Register.h"

struct Register
{
    u8 lower;   
    u8 higher;  ///< Higher 8-bits of the register pair.

    u16 getWord() {
        return (u16) ((higher << 8) | lower); // upper
    }

    void setWord(u16 word) {
        higher = (u8) ((word & 0xFF00) >> 8); // mask to first 4 bits, shift right 8 bits.
        lower = (u8) (word & 0x00FF); // mask to only last 4 bits
    }
};
