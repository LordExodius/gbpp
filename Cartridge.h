#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include "global.h"

struct GBHeader {
    char title[16];
    u8 cartridgeType;
    u16 licenseCode;
};

class Cartridge
{
private:
    u8 *gameData;
    GBHeader getHeader(u8 *gameData);
    long getFileSize(std::string gameFile);
    bool verifyChecksum();
public:
    Cartridge();
    ~Cartridge();

    bool loadCartridge(std::string gameFile);
};

#endif
