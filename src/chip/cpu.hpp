#ifndef CPU_H
#define CPU_H

#include <cstdint>
#include <cstdlib>
#include <unordered_map>
#include <stack>
#include "ram.hpp"
#include "chip_encodings.hpp"
#include "../display/display.hpp"

class CPU
{
private:
    // pointer to a CPU function type
    typedef void (CPU::*cpu_function)(instruction);

    // registers
    uint16_t PC;   // program counter only uses lower 12 bits the size of our memory
    uint16_t I;    // index register holds an address in memmory mostly used to keep track of fonts
    uint8_t dt;    // delay timer decremented at a rate of 60hz
    uint8_t st;    // sound timer decremented at a rate of 60 hz when not zero makes sound
    uint8_t v[16]; // 16 registers the last of which is used as a flag register only 1 or 0

    // other components cpu needs access to
    std::stack<uint16_t> callStack;
    Ram *ram;
    Display *display;
    uint8_t waiting; // set to the v[x] to store in or 0x0F if not waiting
    bool keys[0xF];  // true when a key is currently being pressed false when not

    // sets of cpu instructions for lookup later based on the opcode
    cpu_function operations[16];
    cpu_function setOperations[15];
    std::unordered_map<uint8_t, cpu_function> iOperations;

    // this function fetches the next instruction
    instruction fetch();

public:
    CPU(Ram *ram, Display *display);
    ~CPU();

    void setStart(uint16_t startingPoint);
    void execute();
    void unimplemented(instruction currentInstruction);
    void clearAndReturn(instruction currentInstruction);
    void jump(instruction currentInstruction);
    void ldVx(instruction currentInstruction);
    void addVx(instruction currentInstruction);
    void ldI(instruction currentInstruction);
    void drawI(instruction currentInstruction);
    void call(instruction currentInstruction);
    void skipOnEqual(instruction currentInstruction);
    void skipNotEqual(instruction currentInstruction);
    void skipEqualReg(instruction currentInstruction);
    void sets(instruction currentInstruction);
    void skipNotEqualReg(instruction currentInstruction);
    void addJump(instruction currentInstruction);
    void chipRand(instruction currentInstruction);
    void checkKeyState(instruction currentInstruction);
    void iOps(instruction currentInstruction);

    void ldVxVy(instruction currentInstruction);
    void orVxVy(instruction currentInstruction);
    void andVxVy(instruction currentInstruction);
    void xorVxVy(instruction currentInstruction);
    void addVxVy(instruction currentInstruction);
    void subVxVy(instruction currentInstruction);
    void shiftRightVxVy(instruction currentInstruction);
    void subNotBorrowVxVy(instruction currentInstruction);
    void shiftLeftVxVy(instruction currentInstruction);

    void keyUpdate(uint8_t key, bool pressed);

    void checkDelay(instruction currentInstruction);
    void waitForPress(instruction currentInstruction);
    void setDelay(instruction currentInstruction);
    void setSound(instruction currentInstruction);
    void addI(instruction currentInstruction);
    void setIToFont(instruction currentInstruction);
    void ldBCD(instruction currentInstruction);
    void ldToVx(instruction currentInstruction);
    void ldFromVx(instruction currentInstruction);
};

#endif