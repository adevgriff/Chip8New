#include "cpu.hpp"

CPU::CPU(Ram *ram, Display *display)
{
    PC = 0;
    I = 0;
    dt = 0;
    st = 0;
    waiting = 0x0F;
    for (int i = 0; i < 16; i++)
    {
        v[i] = 0;
    }
    this->ram = ram;
    this->display = display;
    operations[0x0] = &CPU::clearAndReturn;
    operations[0x1] = &CPU::jump;
    operations[0x2] = &CPU::call;
    operations[0x3] = &CPU::skipOnEqual;
    operations[0x4] = &CPU::skipNotEqual;
    operations[0x5] = &CPU::skipEqualReg;
    operations[0x6] = &CPU::ldVx;
    operations[0x7] = &CPU::addVx;
    operations[0x8] = &CPU::sets;
    operations[0x9] = &CPU::skipNotEqualReg;
    operations[0xa] = &CPU::ldI;
    operations[0xb] = &CPU::addJump;
    operations[0xc] = &CPU::chipRand;
    operations[0xd] = &CPU::drawI;
    operations[0xe] = &CPU::checkKeyState;
    operations[0xf] = &CPU::iOps;

    setOperations[0x0] = &CPU::ldVxVy;
    setOperations[0x1] = &CPU::orVxVy;
    setOperations[0x2] = &CPU::andVxVy;
    setOperations[0x3] = &CPU::xorVxVy;
    setOperations[0x4] = &CPU::addVxVy;
    setOperations[0x5] = &CPU::subVxVy;
    setOperations[0x6] = &CPU::shiftRightVxVy;
    setOperations[0x7] = &CPU::subNotBorrowVxVy;
    setOperations[0x8] = &CPU::unimplemented;
    setOperations[0x9] = &CPU::unimplemented;
    setOperations[0xA] = &CPU::unimplemented;
    setOperations[0xB] = &CPU::unimplemented;
    setOperations[0xC] = &CPU::unimplemented;
    setOperations[0xD] = &CPU::unimplemented;
    setOperations[0xE] = &CPU::shiftLeftVxVy;

    iOperations[0x07] = &CPU::checkDelay;
    iOperations[0x0A] = &CPU::waitForPress;
    iOperations[0x15] = &CPU::setDelay;
    iOperations[0x18] = &CPU::setSound;
    iOperations[0x1E] = &CPU::addI;
    iOperations[0x29] = &CPU::setIToFont;
    iOperations[0x33] = &CPU::ldBCD;
    iOperations[0x55] = &CPU::ldFromVx;
    iOperations[0x65] = &CPU::ldToVx;
}
CPU::~CPU()
{
}

void CPU::execute()
{
    if (waiting == 0X0F)
    {
        instruction currentInstruction = fetch();
        (*this.*operations[currentInstruction.byte.nib.one])(currentInstruction);
    }
    if (dt)
    {
        dt--;
    }
    if (st)
    {
        st--;
    }
}

void CPU::setStart(uint16_t startingPoint)
{
    PC = startingPoint;
}

instruction CPU::fetch()
{
    instruction currentInstruction;
    currentInstruction.word = ram->readMSigFirst(PC);
    PC += 2;
    return currentInstruction;
}

void CPU::keyUpdate(uint8_t key, bool pressed)
{
    if (waiting != 0x0F && pressed)
    {
        v[waiting] = key;
        waiting = 0x0F;
    }
    keys[key] = pressed;
}