#include "ram.hpp"

Ram::Ram()
{
   for (int i = 0; i < 80; i++)
   {
      ram[i + 80] = font[i];
   }
}

void Ram::loadProgram(std::string programFile)
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
      int i = 0;
      while (program.read(reinterpret_cast<char *>(&ram[i]), sizeof(uint8_t)) && i < RAMSIZE_BYTES)
      {
         i++;
      }
   }
   program.close();
}