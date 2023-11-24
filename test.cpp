#include <iostream>

#include "CPU.h"
#include "MMU.h"
#include "Cartridge.h"
#include "Emulator.h"
#include "Input.h"

void checkFlags(CPU cpu)
{
    std::cout << "z: " << cpu.getZeroFlag() << "\n";
    std::cout << "n: " << cpu.getSubFlag() << "\n";
    std::cout << "h: " << cpu.getHCarryFlag() << "\n";
    std::cout << "c: " << cpu.getCarryFlag() << "\n\n";
}

int main(int argc, char *argv[])
{
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

    // REGISTERS
    printf("\nREGISTER TESTS\n");
    printf("\nTest 3: CPU successfully initializes register values\n");
    cpu.dumpRegisters();

    printf("\nTest 4: Register F is correctly partitioned into flags\n");
    checkFlags(cpu);

    printf("\nTest 5.1: Zero flag can be fetched and set");
    printf("\nZero: %d\n", cpu.getZeroFlag());
    cpu.setZeroFlag(false);
    checkFlags(cpu);

    printf("\nTest 5.2: Subtraction flag can be fetched and set");
    printf("\nSub: %d\n", cpu.getSubFlag());
    cpu.setSubFlag(true);
    checkFlags(cpu);

    printf("\nTest 5.3: Half carry flag can be fetched and set");
    printf("\nHCarry: %d\n", cpu.getHCarryFlag());
    cpu.setHCarryFlag(false);
    checkFlags(cpu);

    printf("\nTest 5.4: Zero flag can be fetched and set");
    printf("\nCarry: %d\n", cpu.getCarryFlag());
    cpu.setCarryFlag(false);
    checkFlags(cpu);

    printf("\nMEMORY TESTS");
    printf("\nCan write a byte to memory:\n");
    mmu.writeByte(0, 2);
    printf("MMU Memory at 0x00: %d", mmu.readByte(0));

    printf("\nCan write a word to memory:\n");
    mmu.writeByte(0, 0xF);
    printf("MMU Memory at 0x00: %d\n\n", mmu.readWord(0));

    printf("INPUT CLASS TESTS (press 'q' to quit)\n");
    Input input(mmu);
    bool continueLoop = true;

    while (continueLoop) {
        char key;
        printf("Pressing key: ");
        std::cin >> key;
        if (key == 'q') {
            continueLoop = false;
        }
        else {
            input.pressKey(key);
            std::cout << "Button State: " << + input.getButtonKeysState() << std::endl;
            std::cout << "Direction State: " << + input.getDirectionKeysState() << std::endl;

            // Output memory at 0xFF00
            std::cout << "Memory at 0xFF00: " << std::hex << (int)mmu.readByte(0xFF00) << "\n";
            printf("\n");

            input.releaseKey(key);
        }
    }
}