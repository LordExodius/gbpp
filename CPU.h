#ifndef CPU_H_INCLUDED
#define CPU_H_INCLUDED

#include "global.h"
#include "Register.h"
#include "MMU.h"

#define ZERO_VALUE 0x80
#define SUB_VALUE 0x40
#define HALF_VALUE 0x20
#define CARRY_VALUE 0x10

#define DIV_ADDR 0xFF04
#define TIMA_ADDR 0xFF05
#define TMA_ADDR 0xFF06
#define TAC_ADDR 0xFF07

#define DIV_SPEED 16384
#define TAC_0 4096
#define TAC_1 262144
#define TAC_2 65536
#define TAC_3 16384

class CPU
{
private:
    // Registers
    Register AF;    ///< `A` and `F` register pair (Accumulator and Flag registers).
    Register BC;    ///< `B` and `C` register pair.
    Register DE;    ///< `D` and `E` register pair.
    Register HL;    ///< `H` and `L` register pair.
    Register PC;    ///< Program Counter.
    Register SP;    ///< Stack Pointer.

    // Memory
    MMU *mmu;       ///< Pointer to MMU object associated with the emulator.

    // Timer
    int divCounter = 0;     ///< Internal counter used to determine the number of CPU cycles passed before incrementing DIV
    int timerCounter = 0;   ///< Internal counter used to determine the number of CPU cycles passed before incrementing TIMA

public:
    /**
     * @brief Construct a new `CPU` object.
     * 
     * @param mmu Pointer to MMU instance in `Emulator` class.
     */
    CPU(MMU *mmu);

    /**
     * @brief Destroy the `CPU::CPU` object
     */
    ~CPU();

    // REGISTERS
    u16 getSP();
    void setSP(u16 value);
    u16 getPC();
    void setPC(u16 value);

    // Alt
    void pushStackWord(u16 word);
    void pushStackByte(u8 byte);
    u16 popStackWord();
    u8 popStackByte();

    // FLAGS
    bool getZeroFlag();
    void setZeroFlag(bool);
    bool getSubFlag();
    void setSubFlag(bool);
    bool getHCarryFlag();
    void setHCarryFlag(bool);
    bool getCarryFlag();
    void setCarryFlag(bool);

    // Helpers
    bool checkHCarry_8(u8 arg1, u8 arg2, u8 res);
    bool checkHCarry_16(u16 arg1, u16 arg2, u16 res);
    bool checkCarry_8(u8 arg1, u8 arg2);
    bool checkCarry_16(u16 arg1, u16 arg2);

    // Instructions
    u8 getInstruction();
    /**
     * @brief Given an 8-bit CPU instruction, execute the associated Opcode and update flags as necessary.
     * 
     * @param instruction An 8-bit encoded CPU opcode.
     * @return `int` The number of M-cycles taken to execute the opcode.
     */
    int executeInstruction(u8 instruction);

    /**
     * @brief Adds arg to the register A, then stores the result in register A.
     * 
     * @param arg The value to add to register A.
     */
    void add_a(u8 arg);

    /**
     * @brief Subtracts arg to the register A, then stores the result in register A.
     * 
     * @param arg The value to add to register A.
     */
    void sub_a(u8 arg);

    /**
     * @brief these are is not implemented???
     * 
     * @param arg The value to add to register A.
     */
    void add_hl(u16 arg);
    void add_sp(s8 arg);
    void adc();

    /**
     * @brief Increments an 8 bit register.
     * 
     * @param reg The register to increment
     */
    void inc_8(u8 *reg);

    /**
     * @brief Increments an 16 bit register.
     * 
     * @param reg The register to increment
     */
    void inc_16(Register *reg);

    /**
     * @brief Decrements an 8 bit register.
     * 
     * @param reg The register to decrement.
     */
    void dec_8(u8 *reg);

    /**
     * @brief Decrements an 16 bit register.
     * 
     * @param reg The register to decrement.
     */
    void dec_16(Register *reg);

    /**
     * @brief OR operation between register A and a given argument.
     * 
     * @param arg The value to OR against.
     */
    void or_a(u8 arg);

    /**
     * @brief AND operation between register A and a given argument.
     * 
     * @param arg The value to AND against.
     */
    void and_a(u8 arg);

    /**
     * @brief XOR operation between register A and a given argument.
     * 
     * @param arg The value to XOR against.
     */
    void xor_a(u8 arg);

    /**
     * @brief CP operation between register A and a given argument.
     * 
     * @param arg The value to compare against.
     */
    void cp(u8 arg);

    /**
     * @brief POP operation on the given register. Increments the SP.
     * 
     * @param reg The register in which to store the popped value.
     */
    void pop(Register *reg);

    /**
     * @brief JP operation.
     * 
     */
    void jp();

    /**
     * @brief JP HL operation. Sets the PC to the value of HL.
     * 
     */
    void jp_hl();

    /**
     * @brief RET operation. Sets the PC to the value stored in the memory location SP.
     * 
     */
    void ret();

    /**
     * @brief IDK man.
     * 
     */
    void call();

    // Timer
    u8 getDivider();
    u8 getTimer();
    u8 getTimerModulo();
    void resetDivider();
    void setDivider(u8 value);
    void setTimer(u8 value);
    void setTimerModulo(u8 value);
    void updateTimer(int cycles);

    // DEBUG
    void dumpRegisters();
};
#endif