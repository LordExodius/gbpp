#include "Cartridge.h"
#include <string>
#include <cstdio>
#include <filesystem>
#include <cstring>

Cartridge::Cartridge(std::string gameFile) {
    loadCartridge(gameFile);

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
    fileSize = getFileSize(gameFile);
    printf("File size: %ld bytes\n", fileSize);

    // Load ROM into memory
    gameData = new u8[fileSize];
    fread(gameData, 1, fileSize, fp);

    if (gameData == NULL) {
        printf("Unable to allocate memory.\n");
        fclose(fp);
        return false;
    }

    fclose(fp);

    // Read header
    GBHeader header = getHeader(gameData);
    printf("Title: %s\n", header.title);
    printf("Cartridge Type: %d\n", header.cartridgeType);
    printf("License Code: 0x%04X\n", header.licenseCode);

    // Checksum verification: false if corrupted ROM
    if (!verifyChecksum()) {
        printf("Checksum verification failed.\n");
        return false;
    }

    return true;
}

long Cartridge::getFileSize(std::string gameFile) {
    long fileSize = std::filesystem::file_size(gameFile);
    return fileSize;
}

GBHeader Cartridge::getHeader(u8 *gameData) {
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

bool Cartridge::verifyChecksum() {
    u8 checksum = 0;
    for (u16 address = 0x0134; address <= 0x014C; address++) {
        checksum = checksum - gameData[address] - 1;
    }
    u8 storedChecksum = gameData[0x014D];
    return checksum == storedChecksum;
}

u8 Cartridge::getMemory(u16 address) {
    printf("Reading memory at address: 0x%04X\n", address);
    printf("File size: %ld\n", fileSize);
    if (address < fileSize) {
        return gameData[address];
    } else {
        printf("Error: invalid memory address\n");
        return 0;
    }
}

u8 *Cartridge::getGameData() {
    return gameData;
}

Cartridge::~Cartridge() {
    delete[] gameData;
}
