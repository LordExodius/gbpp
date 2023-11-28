#include "MMU.h"

MMU::MMU() {

}

u8 MMU::readByte(u16 location) {
    return memory[location];
}

void MMU::writeByte(u16 location, u8 byte) {
    // if (location == 0xFF04) {
    //     memory[location] = 0x00;
    // }
    if ((location >= 0xE000) && (location < 0xFE00)) {
        writeByte(location - 0x2000, byte);
        memory[location] = byte;
    }
    else if ((location >= 0xFEA0) && (location < 0xFEFF)) {}
    else {
        memory[location] = byte;
    }
}

u16 MMU::readWord(u16 location) {
    u8 lower = readByte(location);
    u8 higher = readByte(location + 1);
    return (u16) ((higher << 8) | lower); // `higher` is automatically promoted to int as per C/C++ spec.
}

void MMU::writeWord(u16 location, u16 word) {
    u8 higher = (u8) ((word & 0xFF00) >> 8); // mask to first 4 bits, shift right 8 bits.
    u8 lower = (u8) (word & 0x00FF); // mask to only last 4 bits
    writeByte(location, lower);
    writeByte(location + 1, higher);
}

MMU::~MMU() {
}
