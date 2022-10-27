#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <unordered_map>
#include <functional>
#include "ram.hpp"
#include "chip_encodings.hpp"
#include "../display/display.hpp"

class CPU
{
private:
    typedef void (CPU::*cpu_function)(instruction);
    uint16_t PC;        // program counter only uses lower 12 bits the size of our memory
    uint16_t I;         // index register holds an address in memmory mostly used to keep track of fonts
    uint8_t SP;         // stack pointer used to call subroutines/functions and return from them
    uint8_t dt;         // delay timer decremented at a rate of 60hz
    uint8_t st;         // sound timer decremented at a rate of 60 hz when not zero makes sound
    uint8_t v[16];      // 16 registers the last of which is used as a flag register only 1 or 0
    uint16_t stack[16]; // 16 holds address in a stack
    Ram *ram;
    Display *display;
    cpu_function operations[16];
    instruction fetch();

public:
    CPU(Ram *ram, Display *display);
    ~CPU();

    void setStart(uint16_t startingPoint);
    void execute();
    void _unimplemented(instruction currentInstruction);
    void _0(instruction currentInstruction);
    void _1(instruction currentInstruction);
    void _6(instruction currentInstruction);
    void _7(instruction currentInstruction);
    void _A(instruction currentInstruction);
    void _D(instruction curretnInstruction);
};

#endif