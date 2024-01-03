/**
 * @class Emulator
 * @brief Main game boy emulator
 * @author Ameena Naqvi
 * The `Emulator` class encapsulates the functionality required to load and execute a specific Game Boy game cartridge.
 * The `Emulator` class follows a structured design, with the `loop()` function serving as the main
 * emulation loop. This loop continuously processes CPU instructions, updates internal timers, and
 * renders graphics, all while simulating the execution of a frame within the Game Boy emulation context.
 **/
#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include "Cartridge.h"
#include "MMU.h"
#include "CPU.h"
#include "Graphics.h"

#define FRAMES_PER_SECOND 60
#define CYCLES_PER_FRAME CPU_CLOCK_SPEED / FRAMES_PER_SECOND

class Emulator
{
private:
    Cartridge cartridge; ///< Cartridge object
    MMU mmu;             ///< MMU object
    CPU cpu;             ///< CPU object
    Graphics *graphics;  ///< Graphics object

public:
    /**
     * @brief Constructor for Emulator object
     * @param fileName name of the gameboy file to be run
     */
    Emulator(const char *fileName);

    /**
     * @brief Destroyer for the Emulator object
     */
    ~Emulator();
    /**
     * @brief Main emulation loop for the emulator.
     * This function simulates the execution of the emulator by continuously processing CPU instructions
     * for a specified number of cycles, representing a frame in the emulation context.
     * The loop fetches the next CPU instruction, executes it, and updates the internal timer based on
     * the number of cycles consumed. The loop continues until the total number of cycles processed
     * reaches the predefined limit per frame (CYCLES_PER_FRAME).
     * After completing the required cycles for a frame, the emulator proceeds to render graphics,
     * providing a visual representation of the current emulation state.
     */
    void loop();
    /**
     * @brief Handles interrupts during emulation.
     * This function is responsible for checking and processing interrupts that may occur during
     * the execution of the emulation loop. It ensures proper handling of interrupts to maintain
     * the accuracy of the emulation.
     */
    void handleInterrupts();
    /**
     * @brief Reads a byte from the specified memory address.
     * @param addr Memory address from which to read the byte.
     * @return The byte read from the specified memory address.
     */
    u8 readMemory(u16 addr);

    /**
     * @brief Main execution for the emulator
     * This function is responsible for running the selected Cartridge and CPU file
     * simulating the processing of instructions and handling various components.
     */
    void run();
};

#endif
