#ifndef EMULATOR_H
#define EMULATOR_H

#include <string>
#include "Cartridge.h"
#include "MMU.h"
#include "CPU.h"
#include "Register.h"
#include "Graphics.h"

#define FRAMES_PER_SECOND 60
#define CYCLES_PER_FRAME CPU_CLOCK_SPEED/FRAMES_PER_SECOND

class Emulator
{
private:
    Cartridge cartridge; ///< Cartridge object
    MMU mmu;             ///< MMU object
    CPU cpu;             ///< CPU object
    Register reg;        ///< Register object
    Graphics *graphics;

public:
    /**
     * @brief Constructor for Emulator object
     */
    Emulator(const char *fileName);

    /**
     * @brief Destroy the Emulator object
     */
    ~Emulator();

    void loop();
    void handleInterrupts();
    void run();
};

#endif
