#include "cpu.hpp"
// 0x00
void CPU::clearAndReturn(instruction currentInstruction)
{
    std::cout << "clearing screen" << std::endl;
    switch (currentInstruction.byte.out.low)
    {
    case 0xE0:
        display->clearScreen();
        break;
    case 0xEE:
        // return from subroutine
        if (!callStack.empty())
        {
            PC = callStack.top();
            callStack.pop();
        }
        else
        {
            std::cout << "problem returning from a subroutine without having anything in the stack" << std::endl;
        }
        break;
    default:
        // unimplemented on purpose not supporting this instruction at the moment
        unimplemented(currentInstruction);
        break;
    }
    return;
}
// 0x01
void CPU::jump(instruction currentInstruction)
{
    std::cout << "setting pc to " << (int)(currentInstruction.word & 0x0FFF) << std::endl;
    PC = currentInstruction.word & 0x0FFF;
    return;
}
// 0x06
void CPU::ldVx(instruction currentInstruction)
{
    std::cout << "setting v[" << (int)currentInstruction.byte.nib.two << "] to " << (int)currentInstruction.byte.out.low << std::endl;
    v[currentInstruction.byte.nib.two] = currentInstruction.byte.out.low;
    return;
}
// 0x07
void CPU::addVx(instruction currentInstruction)
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
// 0x0A
void CPU::ldI(instruction currentInstruction)
{
    std::cout << "setting I to " << (int)(currentInstruction.word & 0x0FFF) << std::endl;
    I = currentInstruction.word & 0x0FFF;
    return;
}
// 0x0D
void CPU::drawI(instruction currentInstruction)
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
// not implemented function
void CPU::unimplemented(instruction currentInstruction)
{
    std::cout << "non existing opcode found " << (int)currentInstruction.word << std::endl;
    return;
}
// 0x02
void CPU::call(instruction currentInstruction)
{
    std::cout << "calling subroutine at " << (int)(currentInstruction.word & 0x0FFF) << std::endl;
    callStack.push(PC);
    PC = currentInstruction.word & 0x0FFF;
}
// 0x03
void CPU::skipOnEqual(instruction currentInstruction)
{
    std::cout << "skipping next inst if V[" << (int)currentInstruction.byte.nib.two << "] == " << (int)currentInstruction.byte.out.low << std::endl;
    if (v[currentInstruction.byte.nib.two] == currentInstruction.byte.out.low)
    {
        PC += 2;
    }
}
// 0x04
void CPU::skipNotEqual(instruction currentInstruction)
{
    std::cout << "skipping next inst if V[" << (int)currentInstruction.byte.nib.two << "] != " << (int)currentInstruction.byte.out.low << std::endl;
    if (v[currentInstruction.byte.nib.two] != currentInstruction.byte.out.low)
    {
        PC += 2;
    }
}
// 0x05
void CPU::skipEqualReg(instruction currentInstruction)
{
    std::cout << "skipping next inst if V[" << (int)currentInstruction.byte.nib.two << "] == "
              << "v[" << (int)currentInstruction.byte.nib.three << "]" << std::endl;
    if (v[currentInstruction.byte.nib.two] == v[currentInstruction.byte.nib.three])
    {
        PC += 2;
    }
}

// 0x08
void CPU::sets(instruction currentInstruction)
{
    (this->*setOperations[currentInstruction.byte.nib.four])(currentInstruction);
}

// 0x8xy0
void CPU::ldVxVy(instruction currentInstruction)
{
    std::cout << "stores V[" << (int)currentInstruction.byte.nib.three << "] into "
              << "v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    v[currentInstruction.byte.nib.two] = v[currentInstruction.byte.nib.three];
}

// 0x8xy1
void CPU::orVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself or'd with "
              << "v[" << (int)currentInstruction.byte.nib.three << "]" << std::endl;
    v[currentInstruction.byte.nib.two] |= v[currentInstruction.byte.nib.three];
}

// 0x8xy2
void CPU::andVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself anded with "
              << "v[" << (int)currentInstruction.byte.nib.three << "]" << std::endl;
    v[currentInstruction.byte.nib.two] &= v[currentInstruction.byte.nib.three];
}

// 0x8xy3
void CPU::xorVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself xor'd with "
              << "v[" << (int)currentInstruction.byte.nib.three << "]" << std::endl;
    v[currentInstruction.byte.nib.two] ^= v[currentInstruction.byte.nib.three];
}

// 0x8xy4
void CPU::addVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself added with "
              << "v[" << (int)currentInstruction.byte.nib.three << "] ";
    uint16_t result = v[currentInstruction.byte.nib.two] + v[currentInstruction.byte.nib.three];
    if (result > 0xFF)
    {
        std::cout << "and v[f] is set" << std::endl;
        v[0xF] = 1;
    }
    else
    {
        std::cout << "and V[F] not set" << std::endl;
        v[0xF] = 0;
    }
    v[currentInstruction.byte.nib.two] = result & 0x00FF;
}

// 0x8xy5
void CPU::subVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself subtracted with "
              << "v[" << (int)currentInstruction.byte.nib.three << "] ";
    uint16_t result = v[currentInstruction.byte.nib.two];
    if (result < v[currentInstruction.byte.nib.three])
    {
        std::cout << "flag not set" << std::endl;
        v[0xF] = 0;
        result = result + 0b100000000;
    }
    else
    {
        std::cout << "flag is set" << std::endl;
        v[0xF] = 1;
    }
    v[currentInstruction.byte.nib.two] = result - v[currentInstruction.byte.nib.three];
}

// 0x8xy6
void CPU::shiftRightVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself shifted right "
              << "carry set to least significant bit" << std::endl;
    v[0xF] = v[currentInstruction.byte.nib.two] & 0x1;
    v[currentInstruction.byte.nib.two] = v[currentInstruction.byte.nib.two] >> 1;
}

// 0x8xy7
void CPU::subNotBorrowVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself subtracting "
              << "v[" << (int)currentInstruction.byte.nib.three << "] ";
    uint16_t result = v[currentInstruction.byte.nib.three];
    if (result < v[currentInstruction.byte.nib.two])
    {
        std::cout << "flag not set" << std::endl;
        v[0xF] = 0;
        result = result + 0b100000000;
    }
    else
    {
        std::cout << "flag is set" << std::endl;
        v[0xF] = 1;
    }
    v[currentInstruction.byte.nib.two] = result - v[currentInstruction.byte.nib.two];
}

// 0x8xyE
void CPU::shiftLeftVxVy(instruction currentInstruction)
{
    std::cout << "stores into V[" << (int)currentInstruction.byte.nib.two << "] itself shifted left "
              << "carry set to most significant bit" << std::endl;
    v[0xF] = (v[currentInstruction.byte.nib.two] >> 7) & 0x1;
    v[currentInstruction.byte.nib.two] = v[currentInstruction.byte.nib.two] << 1;
}

// 0x09
void CPU::skipNotEqualReg(instruction currentInstruction)
{
    std::cout << "skip if V[" << (int)currentInstruction.byte.nib.two << "] no equal to "
              << "v[" << (int)currentInstruction.byte.nib.three << "] " << std::endl;
    if (v[currentInstruction.byte.nib.two] != v[currentInstruction.byte.nib.three])
    {
        PC += 2;
    }
}

void CPU::addJump(instruction currentInstruction)
{
    std::cout << "jump to " << (int)(currentInstruction.word & 0x0FFF) << " + v[0]" << std::endl;
    PC = v[0x00] + (currentInstruction.word & 0x0FFF);
}

void CPU::chipRand(instruction currentInstruction)
{
    std::cout << "set v[" << (int)currentInstruction.byte.nib.two << "] to a random value" << std::endl;
    v[currentInstruction.byte.nib.two] = (currentInstruction.byte.out.low & (rand() % 0x100));
}

// 0x0E checkkeystate
void CPU::checkKeyState(instruction currentInstruction)
{
    switch (currentInstruction.byte.out.low)
    {
    case 0x9E: // vx is pressed
        std::cout << "skip instruction if key " << (int)v[currentInstruction.byte.nib.two] << " is pressed" << std::endl;
        if (keys[v[currentInstruction.byte.nib.two]])
        {
            PC += 2;
        }
        break;
    case 0xA1: // vx is not pressed
        std::cout << "skip instruction if key " << (int)v[currentInstruction.byte.nib.two] << " is not pressed" << std::endl;
        if (!keys[v[currentInstruction.byte.nib.two]])
        {
            PC += 2;
        }
        break;
    default:
        unimplemented(currentInstruction);
        break;
    }
}

// 0x0F
void CPU::iOps(instruction currentInstruction)
{
    if (iOperations.find(currentInstruction.byte.out.low) != iOperations.end())
    {
        (this->*iOperations[currentInstruction.byte.out.low])(currentInstruction);
    }
    else
    {
        unimplemented(currentInstruction);
    }
}

// 0xFx07
void CPU::checkDelay(instruction currentInstruction)
{
    std::cout << "setting v[" << (int)currentInstruction.byte.nib.two << "] to the value in the delay timer which is " << (int)dt << std::endl;
    v[currentInstruction.byte.nib.two] = dt;
}

void CPU::waitForPress(instruction currentInstruction)
{
    std::cout << "halting execution until key is pressed storing the key in v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    waiting = currentInstruction.byte.nib.two;
}

void CPU::setDelay(instruction currentInstruction)
{
    std::cout << "set the delay timer to the value in v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    dt = v[currentInstruction.byte.nib.two];
}

void CPU::setSound(instruction currentInstruction)
{
    std::cout << "set the sound timer to the value in v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    st = v[currentInstruction.byte.nib.two];
}

void CPU::addI(instruction currentInstruction)
{
    std::cout << "set I to the value of I + the value in v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    I = I + v[currentInstruction.byte.nib.two];
}

void CPU::setIToFont(instruction currentInstruction)
{
    std::cout << "set I to the location in ram of the Font character corresponding to v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    I = START_OF_FONT + (5 * v[currentInstruction.byte.nib.two]);
}

void CPU::ldBCD(instruction currentInstruction)
{
    std::cout << "decimal representation of v[" << (int)currentInstruction.byte.nib.two << "] stored in I, I+1, and I+2" << std::endl;
    ram->writeAddress(v[currentInstruction.byte.nib.two] / 100, I);
    ram->writeAddress((v[currentInstruction.byte.nib.two] / 10) % 10, I + 1);
    ram->writeAddress(v[currentInstruction.byte.nib.two] % 10, I + 2);
}

void CPU::ldFromVx(instruction currentInstruction)
{
    std::cout << "storing v[0] - v[" << (int)currentInstruction.byte.nib.two << "] into I + (v index)" << std::endl;
    for (int i = 0; i <= (int)currentInstruction.byte.nib.two; i++)
    {
        ram->writeAddress(v[i], I + i);
    }
}

void CPU::ldToVx(instruction currentInstruction)
{
    std::cout << "storing I values into v[0] - v[" << (int)currentInstruction.byte.nib.two << "]" << std::endl;
    for (int i = 0; i <= (int)currentInstruction.byte.nib.two; i++)
    {
        v[i] = ram->readByte(I + i);
    }
}