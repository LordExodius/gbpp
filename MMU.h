/**
 * @class MMU
 * @brief Memory Management Unit for Emulator
 * @author Lesley Hou
 * This class represents the Memory Management Unit (MMU) for an emulator object.
 * It is responsible for handling memory-related operations, such as reading and writing bytes and words.
 */

#ifndef MMU_H
#define MMU_H

#include "global.h"
#include "Cartridge.h"

class MMU
{
private:
    /**
     * @brief Memory array to store bytes for the emulator.
     *
     */
    u8 memory[0xFFFF]; ///< Gameboy Memory
public:
    /**
     * @brief Constructor for MMU object
     * @param void
     */
    MMU(Cartridge *cartridge, std::string gameFile);
    /**
     * @brief Destroyer for MMU object
     * @param void
     */
    ~MMU();
    /**
     * @brief Reads an 8 bit byte from the specified memory location
     *
     * @param location The memory location from which to read the byte
     * @return u8 The 8-bit byte read from the specified location
     */
    u8 readByte(u16 location);
    /**
     * @brief Writes an 8-bit byte to the specified memory location
     *
     * @param location The memory location to which the byte will be written
     * @param data The 8-bit data to be written to the specified location
     */
    void writeByte(u16 location, u8 data);
    /**
     * @brief Reads a 16-bit word from the specified memory location
     *
     * @param location The memory location from which to read the word
     * @return u16 The 16-bit word read from the specified location
     */
    u16 readWord(u16 location);
    /**
     * @brief Write a 16-bit word to the specified memory location
     *
     * @param location The memory location to which the word will be written
     * @param data The 16-bit data to be written to the specified location
     */
    void writeWord(u16 location, u16 data);
};

#endif
