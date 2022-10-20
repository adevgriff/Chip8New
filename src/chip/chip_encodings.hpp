#ifndef CHIP_ENCODINGS_H
#define CHIP_ENCODINGS_H

#include <cstdint>

union instruction
{
   union bytes
   {
      struct outside
      {
         uint8_t low;
         uint8_t high;
      } out;
      struct inside
      {
         uint8_t hnib : 4;
         uint8_t mid : 8;
         uint8_t lnib : 4;
      } in;
      struct nibs
      {
         uint8_t four : 4;
         uint8_t three : 4;
         uint8_t two : 4;
         uint8_t one : 4;
      } nib;
   } byte;
   uint16_t word;
};

typedef instruction instruction;

#endif
