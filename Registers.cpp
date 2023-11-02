#include <stdint.h>

class Registers
{
private:
    /* data */
    uint8_t A, B, C, D, E, F, H, L;
    uint16_t PC, SP;

public:
    Registers(/* args */);
    ~Registers();

};

Registers::Registers(/* args */)
{
}

Registers::~Registers()
{
}
