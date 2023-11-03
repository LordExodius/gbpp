#include "Register.h"

class CPU
{
private:
    // Registers
    Register AF;
    Register BC;
    Register DE;
    Register HL;
    Register PC;
    Register SP;

public:
    CPU(/* args */);
    ~CPU();

    // Debug
    void resetRegisters();
    void printRegisters();
};
