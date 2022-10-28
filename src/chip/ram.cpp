#include "ram.hpp"

Ram::Ram()
{
   for (int i = 0; i < 80; i++)
   {
      ram[i + START_OF_FONT] = font[i];
   }
}

void Ram::loadProgram(std::string programFile, unsigned int startPoint)
{
   std::ifstream program;
   program.open(programFile, std::ifstream::binary);

   if (program.fail())
   {
      // could not open the file
      std::cout << "Could not open the program to load into memory" << std::endl;
   }
   else
   {
      int i = startPoint;
      while (program.read(reinterpret_cast<char *>(&ram[i]), sizeof(uint8_t)) && i < RAMSIZE_BYTES)
      {
         i++;
      }
   }
   program.close();
}

void Ram::printRam()
{
   for (int i = 0; i < RAMSIZE_BYTES; i++)
   {
      std::cout << ram[i];
   }
}

void Ram::writeAddress(uint8_t byte, uint16_t address)
{
   address = address & 0x0fff;
   ram[address] = byte;
}

uint8_t Ram::readByte(uint16_t address)
{
   address = address & 0x0fff;
   return ram[address];
}

uint16_t Ram::readLSigFirst(uint16_t address)
{
   address = address & 0x0fff;
   if (address > 0x0FFE)
   {
      // address is to big for this operation
      std::cout << "issue reading to values at last value in memory" << std::endl;
      return 0;
   }
   else
   {
      return ram[address + 1] << 8 | ram[address];
   }
}

uint16_t Ram::readMSigFirst(uint16_t address)
{
   address = address & 0x0fff;
   if (address > 0x0FFE)
   {
      // address is to big for this operation
      std::cout << "issue reading to values at last value in memory" << std::endl;
   }
   else
   {
      return (uint16_t)ram[address] << 8 | (uint16_t)ram[address + 1];
   }
}