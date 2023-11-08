#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>

struct GBHeader {
    char title[16];
    uint8_t cartridgeType;
    uint16_t licenseCode;
};

class Cartridge
{
private:
    uint8_t *gameData;
    long getFileSize(std::string gameFile);
    GBHeader getHeader(uint8_t *gameData);
public:
    Cartridge();
    ~Cartridge();

    bool loadCartridge(std::string gameFile);
};

#endif
