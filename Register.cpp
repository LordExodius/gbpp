#include "Register.h"

// Debug
#include <iostream>
#include <bitset>


u16 Register::getWord() {
    return (u16) ((higher << 8) | lower); // upper
}

void Register::setWord(u16 word) {
    higher = (u8) ((word & 0xFF00) >> 8); // mask to first 4 bits, shift right 8 bits.
    lower = (u8) (word & 0x00FF); // mask to only last 4 bits

    #if DEBUG == true
    std::cout << "word:" << std::hex << +word << "\n";
    std::cout << "higher:" << std::hex << +higher << "\n";
    std::cout << "lower:" << std::hex << +lower << "\n";
    #endif
}