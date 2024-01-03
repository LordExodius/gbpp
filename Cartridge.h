/**
 * @class Cartridge
 * @brief Game Boy Cartridge called by the Emulator
 * @author Ameena Naqvi
 * This class loads a specific game cartridge based on the name specified by the emulator class
 */
#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <string>
#include "global.h"

/**
 * @brief A header struct. Contains the title of the ROM file,
 * the cartridge type, and the license code.
 */
struct GBHeader
{
    char title[16];
    u8 cartridgeType;
    u16 licenseCode;
};

class Cartridge
{
private:
    u8 *gameData;  ///< Pointer to the ROM file data as an array of bytes.
    long fileSize; ///< Size of the ROM file in bytes.

    /**
     * @brief Get the header values of the ROM file.
     *
     * @param gameData A pointer to the ROM file data.
     * @return GBHeader, a struct containing the necessary header values
     */
    GBHeader getHeader(u8 *gameData);

    /**
     * @brief Get the size of the ROM file in bytes.
     *
     * @param gameFile A string containing the name of the ROM file.
     * @return A long value representing the size of the ROM file in bytes.
     */
    long getFileSize(std::string gameFile);

    /**
     * @brief Verifies the checksum of the ROM file to ensure the game files
=======
     * @brief Verifies the checksum of the ROM file to ensure the game files
>>>>>>> 11b6004329bdab6e6d80e106cc5e21d32024e9a5
     * are not corrupted.
     *
     * @return true if the checksum is valid, false if the checksum is invalid
     */
    bool verifyChecksum();

public:
    /**
     * @brief Constructor for Cartridge object
     */
    Cartridge(std::string gameFile);

    /**
     * @brief Destroy the Cartridge object
     */
    ~Cartridge();

    /**
     * @brief Get the size of the ROM file in bytes.
     *
     * @param gameFile A string containing the name of the ROM file.
     * @return A long value representing the size of the ROM file in bytes.
     */
    long getFileSize(std::string gameFile);

    /**
     * @brief Getter method for gameData.
     *
     * @return A pointer to the ROM file data as an array of bytes.
     */
    u8 *getGameData();

    /**
     * @brief Gets a byte of memory at the specified address.
     *
     * @param address A 16-bit memory address.
     * @return A byte of memory at the address indicated in the param
     */
    u8 getMemory(u16 address);

    /**
     * @brief Read the ROM file and load it into memory.
     *
     * @param gameFile A string containing the name of the ROM file.
     * @return true if the ROM file can be read and loaded, false otherwise.
     */
    bool loadCartridge(std::string gameFile);
};

#endif
