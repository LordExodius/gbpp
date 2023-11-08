#include <iostream>

#include "CPU.h"
#include "MMU.h"

void checkFlags(CPU cpu) {
    std::cout << "z: " << cpu.getZeroFlag() << "\n";
    std::cout << "n: " << cpu.getSubFlag() << "\n";
    std::cout << "h: " << cpu.getHCarryFlag() << "\n";
    std::cout << "c: " << cpu.getCarryFlag() << "\n\n";
}

int main() {
    CPU cpu;

    cpu.dumpRegisters();

    checkFlags(cpu);
    cpu.setZeroFlag(false);
    checkFlags(cpu);
    cpu.setSubFlag(true);
    checkFlags(cpu);

    cpu.dumpRegisters();
}