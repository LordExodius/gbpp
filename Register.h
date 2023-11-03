#include <cstdint>

using u8 = std::uint8_t;
using u16 = std::uint16_t;

struct Register
{
    u8 lower;               // F, C, E, L
    u8 higher;               // A, B, D, H
    u16 getWord();          // combine lower and upper
    void setWord(u16);      // split u16
};