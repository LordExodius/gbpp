#include <iostream>
#include <fstream>
#include "CPU.h"
#include "MMU.h"
#include "Cartridge.h"
#include "Emulator.h"
#include "Graphics.h"
#include "Input.h"

// Unit Testing
#define CONFIG_CATCH_MAIN
#include "catch_amalgamated.hpp"

TEST_CASE("Input class tests") {
    Cartridge cartridge("Tetris.gb");
    MMU mmu(&cartridge, "Tetris.gb");

    SECTION("Pressing and releasing keys") {
        Input input(mmu);

        // Simulate key press for direction key
        // "Select" button must be pressed while reading directional keys (lower nibble)
        // Therefore button key state should be 14 (1110 in bitwise)
        // and direction key state should be 13 (1101 in bitwise as 'a' maps to position 1)
        input.pressKey('a');
        REQUIRE(input.getButtonKeysState() == 14);
        REQUIRE(input.getDirectionKeysState() == 13);

        // Simulate key release 
        // 15 in bitwise is 1111, so all keys are unpressed
        input.releaseKey('a');
        REQUIRE(input.getButtonKeysState() ==  15);
        REQUIRE(input.getDirectionKeysState() == 15);

        // Simulate key press for button key that is not mapped
        // All keys should be unpressed
        input.pressKey('-');
        REQUIRE(input.getButtonKeysState() == 15);
        REQUIRE(input.getDirectionKeysState() == 15);
    }

    SECTION("Check memory at 0xFF00") {
        Input input(mmu);

        // Simulate key press for a mapped key
        // 222 in bitwise is 11011110, so all keys are unpressed except 'a'
        input.pressKey('a');
        REQUIRE(static_cast<int>(mmu.readByte(0xFF00)) == 222);

        // Simulate key release
        // 255 in bitwise is 11111111, so all keys are unpressed
        input.releaseKey('a');
        REQUIRE(static_cast<int>(mmu.readByte(0xFF00)) == 255);

        // Simulate key press for an mapped key
        input.pressKey('=');
        REQUIRE(static_cast<int>(mmu.readByte(0xFF00)) == 255);

        // Simulate release of the wrong key - should still be in "pressed" state
        input.pressKey('a');
        input.releaseKey('0');
        REQUIRE(static_cast<int>(mmu.readByte(0xFF00)) == 222);

    }
}

TEST_CASE("F register flags are accessible and initialized correctly") {
    Emulator emu("Tetris.gb");
    // Register F init to 0xB0 (0b10110000)
    REQUIRE(emu.getCPU()->getZeroFlag() == true);
    REQUIRE(emu.getCPU()->getSubFlag() == false);
    REQUIRE(emu.getCPU()->getHCarryFlag() == true);
    REQUIRE(emu.getCPU()->getCarryFlag() == true);
}

// TEST_CASE("Cartridge returns correctly for valid and invalid ROM files") {
//     Cartridge cart1;
//     REQUIRE(cart1.loadCartridge("Tetris.gb") == true);
//     Cartridge cart2;
//     REQUIRE(cart2.loadCartridge("GAJGLSDHGSHL") == false);
// }

// TEST_CASE("Run main gameplay loop") {
//     Emulator emu("Tetris.gb");
//     // while (true) {
//     //     emu.loop();
//     //     while (std::cin.get() != '\n');
//     // }
// }

TEST_CASE("Verify register access and write") {
    Emulator emu("Tetris.gb");
    emu.getCPU()->resetRegisters();
    REQUIRE(emu.getCPU()->getAF() == 0x0000);
    REQUIRE(emu.getCPU()->getBC() == 0x0000);
    REQUIRE(emu.getCPU()->getDE() == 0x0000);
    REQUIRE(emu.getCPU()->getHL() == 0x0000);
    REQUIRE(emu.getCPU()->getSP() == 0x0000);
    REQUIRE(emu.getCPU()->getPC() == 0x0000);
    emu.getCPU()->setAF(0x1000);
    REQUIRE(emu.getCPU()->getAF() == 0x1000);
    emu.getCPU()->setBC(0x1100);
    REQUIRE(emu.getCPU()->getBC() == 0x1100);
    emu.getCPU()->setDE(0x1200);
    REQUIRE(emu.getCPU()->getDE() == 0x1200);
    emu.getCPU()->setHL(0x1300);
    REQUIRE(emu.getCPU()->getHL() == 0x1300);
    emu.getCPU()->setSP(0x1400);
    REQUIRE(emu.getCPU()->getSP() == 0x1400);
    emu.getCPU()->setPC(0x1500);
    REQUIRE(emu.getCPU()->getPC() == 0x1500);
}

// TEST_CASE("Export memory to file") {
//     std::fstream file;
//     file.open("memory.txt", std::ios::out);
//     Emulator emu("Tetris.gb");
//     for (u16 i = 0; i < 0xFFFF; i++) {
//         file << "0x" << std::hex << i << ": 0x" << std::hex << (int)emu.readMemory(i) << "\n";
//     }
//     while (std::cin.get() != '\n') {};
// }
// int main(int argc, char *argv[]) {
//     printf("EMULATOR TESTS\n");
//     printf("Test 1: User can load game file using command line argument\n");
//     const char *fileName = argv[1];
//     Emulator emulator(fileName);

//     printf("\nCARTRIDGE TESTS\n");
//     printf("Test 1: User can load ROM file and read header\n");
//     Cartridge cartridge;
//     cartridge.loadCartridge("Tetris.gb");

//     printf("\nTest 2: Can access cartridge memory given a 16-bit address\n");
//     std::cout << "Cartridge memory at 0x008: " << std::hex << (int)cartridge.getMemory(0x008) << "\n";

//     printf("\nCPU TESTS\n");
//     MMU mmu;
//     CPU cpu(&mmu);

//     // REGISTERS
//     printf("\nREGISTER TESTS\n");
//     printf("\nTest 3: CPU successfully initializes register values\n");
//     cpu.dumpRegisters();

//     printf("\nTest 4: Register F is correctly partitioned into flags\n");
//     checkFlags(cpu);

//     printf("\nTest 5.1: Zero flag can be fetched and set");
//     printf("\nZero: %d\n", cpu.getZeroFlag());
//     cpu.setZeroFlag(false);
//     checkFlags(cpu);

//     printf("\nTest 5.2: Subtraction flag can be fetched and set");
//     printf("\nSub: %d\n", cpu.getSubFlag());
//     cpu.setSubFlag(true);
//     checkFlags(cpu);

//     printf("\nTest 5.3: Half carry flag can be fetched and set");
//     printf("\nHCarry: %d\n", cpu.getHCarryFlag());
//     cpu.setHCarryFlag(false);
//     checkFlags(cpu);

//     printf("\nTest 5.4: Zero flag can be fetched and set");
//     printf("\nCarry: %d\n", cpu.getCarryFlag());
//     cpu.setCarryFlag(false);
//     checkFlags(cpu);

//     printf("\nMEMORY TESTS");
//     printf("\nCan write a byte to memory:\n");
//     mmu.writeByte(0, 2);
//     printf("MMU Memory at 0x00: %d", mmu.readByte(0));

//     printf("\nCan write a word to memory:\n");
//     mmu.writeByte(0, 0xF);
//     printf("MMU Memory at 0x00: %d\n\n", mmu.readWord(0));
// }