#include "Cartridge.h"
#include <string>
#include <cstdio>
#include <filesystem>
#include <cstring>

Cartridge::Cartridge() {
    gameData = nullptr;
}

bool Cartridge::loadCartridge(std::string gameFile) {
    printf("Loading ROM file: %s\n", gameFile.c_str());

    //  Open ROM file
    FILE *fp = fopen(gameFile.c_str(), "rb");

    if (!fp) {
        printf("Cannot open file.\n");
        return false;
    }

    // Determine file size
    long fileSize = getFileSize(gameFile);
    printf("File size: %ld bytes\n", fileSize);

    // Checksum verification
    // TODO

    // Load ROM into memory
    gameData = new uint8_t[fileSize];
    fread(gameData, 1, fileSize, fp);

    if (gameData == NULL) {
        printf("Unable to allocate memory.\n");
    }

    fclose(fp);

    // Read header
    GBHeader header = getHeader(gameData);
    printf("Title: %s\n", header.title);
    printf("Cartridge Type: %d\n", header.cartridgeType);
    printf("License Code: 0x%04X\n", header.licenseCode);

    return true;
}

long Cartridge::getFileSize(std::string gameFile) {
    long fileSize = std::filesystem::file_size(gameFile);
    return fileSize;
}

GBHeader Cartridge::getHeader(uint8_t *gameData) {
    GBHeader header;
    
    // Header offset values
    const int titleOffset = 0x0134;
    const int cartridgeTypeOffset = 0x0147;
    const int licenseCodeOffset = 0x0144;

    std::memcpy(header.title, gameData + titleOffset, 16);
    header.cartridgeType = gameData[cartridgeTypeOffset];
    header.licenseCode = gameData[licenseCodeOffset] | (gameData[licenseCodeOffset + 1] << 8);

    return header;
}

Cartridge::~Cartridge() {
    delete[] gameData;
}
