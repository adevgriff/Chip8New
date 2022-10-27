#ifndef DISPLAY_H
#define DISPLAY_H

#include "../constants.hpp"
#include <glad/glad.h>

void setupDisplayVertices(float vertices[SCREEN_VERTICES]);
void setupDisplayIndeices(unsigned int indices[TOTAL_INDICES]);

class Display
{
private:
   bool display[SCREEN_WIDTH][SCREEN_HEIGHT];

public:
   Display();

   bool getValue(int x, int y);
   void setValue(int x, int y, bool value);

   void clearScreen();
   void drawScreen();
   void flipBit(int x, int y);
};

#endif