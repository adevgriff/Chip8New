#include "cpu.hpp"

CPU::CPU(Ram *ram, Display *display)
{
    PC = 0;
    I = 0;
    SP = 0;
    dt = 0;
    st = 0;
    for (int i = 0; i < 16; i++)
    {
        v[i] = 0;
        stack[i] = 0;
    }
    this->ram = ram;
    this->display = display;
    operations[0] = &CPU::_0;
    operations[1] = &CPU::_1;
    operations[2] = &CPU::_unimplemented;
    operations[3] = &CPU::_unimplemented;
    operations[4] = &CPU::_unimplemented;
    operations[5] = &CPU::_unimplemented;
    operations[6] = &CPU::_6;
    operations[7] = &CPU::_7;
    operations[9] = &CPU::_unimplemented;
    operations[8] = &CPU::_unimplemented;
    operations[0xa] = &CPU::_A;
    operations[0xb] = &CPU::_unimplemented;
    operations[0xc] = &CPU::_unimplemented;
    operations[0xd] = &CPU::_D;
    operations[0xe] = &CPU::_unimplemented;
    operations[0xf] = &CPU::_unimplemented;
}
CPU::~CPU()
{
}

void CPU::execute()
{
    instruction currentInstruction = fetch();
    (*this.*operations[currentInstruction.byte.nib.one])(currentInstruction);
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

void CPU::_0(instruction currentInstruction)
{
    std::cout << "clearing screen" << std::endl;
    switch (currentInstruction.byte.out.low)
    {
    case 0xE0:
        display->clearScreen();
        break;
    }
    return;
}

void CPU::_1(instruction currentInstruction)
{
    std::cout << "setting pc to " << (int)(currentInstruction.word & 0x0FFF) << std::endl;
    PC = currentInstruction.word & 0x0FFF;
    return;
}
void CPU::_6(instruction currentInstruction)
{
    std::cout << "setting v[" << (int)currentInstruction.byte.nib.two << "] to " << (int)currentInstruction.byte.out.low << std::endl;
    v[currentInstruction.byte.nib.two] = currentInstruction.byte.out.low;
    return;
}
void CPU::_7(instruction currentInstruction)
{
    std::cout << "adding " << (int)currentInstruction.byte.out.low << " to " << (int)currentInstruction.byte.nib.two;
    if (!(currentInstruction.byte.nib.four == 1 && v[currentInstruction.byte.nib.two] == 0xFF))
    {
        std::cout << " and affecting the carry";
        if (v[currentInstruction.byte.nib.two] + currentInstruction.byte.out.low > 0xFF)
        {
            v[15] = 1;
        }
        else
        {
            v[15] = 0;
        }
    }
    std::cout << std::endl;
    v[currentInstruction.byte.nib.two] = (v[currentInstruction.byte.nib.two] + currentInstruction.byte.out.low) & 0xFF;
    return;
}

void CPU::_A(instruction currentInstruction)
{
    std::cout << "setting I to " << (int)(currentInstruction.word & 0x0FFF) << std::endl;
    I = currentInstruction.word & 0x0FFF;
    return;
}

void CPU::_D(instruction currentInstruction)
{
    int x = v[currentInstruction.byte.nib.two] % 64;
    int y = v[currentInstruction.byte.nib.three] % 32;
    std::cout << "drawing at " << x << "," << y << std::endl;
    v[15] = 0;
    for (int n = 0; n < currentInstruction.byte.nib.four; n++)
    {
        int row = ram->readByte(I + n);
        for (int bit = 7; bit >= 0; bit--)
        {
            if (row & (1 << bit))
            {
                display->flipBit(x + 7 - bit, y + n);
            }
        }
    }
    return;
}

void CPU::_unimplemented(instruction currentInstruction)
{
    std::cout << "non existing opcode found " << (int)currentInstruction.word << std::endl;
    return;
}