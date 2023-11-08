#include <iostream>

#include "CPU.h"
#include "MMU.h"
#include "Cartridge.h"
#include "Emulator.h"

void checkFlags(CPU cpu) {
    std::cout << "z: " << cpu.getZeroFlag() << "\n";
    std::cout << "n: " << cpu.getSubFlag() << "\n";
    std::cout << "h: " << cpu.getHCarryFlag() << "\n";
    std::cout << "c: " << cpu.getCarryFlag() << "\n\n";
}

int main(int argc, char *argv[]) {
    printf("EMULATOR TESTS\n");
    printf("Test 1: User can load game file using command line argument\n");
    const char *fileName = argv[1];
    Emulator emulator(fileName);

    printf("\nCARTRIDGE TESTS\n");
    printf("Test 1: User can load ROM file and read header\n");
    Cartridge cartridge;
    cartridge.loadCartridge("Tetris.gb");

    printf("\nTest 2: Can access cartridge memory given a 16-bit address\n");
    std::cout << "Cartridge memory at 0x008: " << std::hex << (int)cartridge.getMemory(0x008) << "\n";

    printf("\nCPU TESTS\n");
    MMU mmu;
    CPU cpu(&mmu);

    cpu.dumpRegisters();

    checkFlags(cpu);
    cpu.setZeroFlag(false);
    checkFlags(cpu);
    cpu.setSubFlag(true);
    checkFlags(cpu);

    cpu.dumpRegisters();
}