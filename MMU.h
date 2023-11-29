#ifndef MMU_H
#define MMU_H

#include "global.h"
#include "Cartridge.h"

class MMU
{
private:
    u8 memory[0xFFFF]; ///< Gameboy Memory
public:
    MMU(Cartridge *cartridge, std::string gameFile);
    ~MMU();

    u8 readByte(u16 location);
    void writeByte(u16 location, u8 data);
    u16 readWord(u16 location);
    void writeWord(u16 location, u16 data);
};

#endif
