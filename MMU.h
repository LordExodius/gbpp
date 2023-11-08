#ifndef MMU_H
#define MMU_H

#include "global.h"

class MMU
{
private:
    u8 memory[0x10000]; ///< Gameboy Memory
public:
    MMU(/* args */);
    ~MMU();

    u8 readByte(u16 location);
    void writeByte(u16 location, u8 data);
    u16 readWord(u16 location);
    void writeWord(u16 location, u16 data);
};

#endif
