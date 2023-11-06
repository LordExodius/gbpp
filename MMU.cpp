#include "MMU.h"

MMU::MMU() {

}

u8 MMU::readByte(u16 location) {
    return memory[location];
}

void MMU::writeByte(u16 location, u8 data) {

}

u16 MMU::readWord(u16 location) {
    u8 lower = memory[location];
    u8 higher = memory[location + 1];
    return (u16) ((higher << 8) | lower);
}