#include "display.hpp"

void setupDisplayVertices(float vertices[SCREEN_VERTICES])
{
   float xPos, yPos;
   xPos = -1.0f;
   yPos = 1.0f;
   for (int y = 0; y < SCREEN_HEIGHT + 1; y++)
   {
      for (int x = 0; x < SCREEN_WIDTH + 1; x++)
      {
         int offset = (y * (SCREEN_WIDTH + 1) + x) * 3;
         // set the x coord of the vertex
         vertices[offset] = xPos;
         xPos = xPos + (float)(2.0f / SCREEN_WIDTH);

         // set the y coord of the vertex
         vertices[offset + 1] = yPos;

         // set the z coord of the vertex will go unused
         vertices[offset + 2] = 0.0f;
      }
      xPos = -1.0f;
      yPos = yPos - (float)(2.0f / SCREEN_HEIGHT);
   }
}

void setupDisplayIndeices(unsigned int indices[TOTAL_INDICES])
{
   for (int y = 0; y < SCREEN_HEIGHT; y++)
   {
      for (int x = 0; x < SCREEN_WIDTH; x++)
      {
         // per screen pixel square size of PIXEL_SIZE setup indices
         int offset = ((y * SCREEN_WIDTH) + x) * 6;

         // find vertexes
         int a, b, c, d;
         a = (y * SCREEN_WIDTH) + x + y;
         b = (y * SCREEN_WIDTH) + (x + 1) + y;
         c = ((y + 1) * SCREEN_WIDTH) + x + (y + 1);
         d = ((y + 1) * SCREEN_WIDTH) + (x + 1) + (y + 1);

         // first triangle
         indices[offset] = a;
         indices[offset + 1] = b;
         indices[offset + 2] = c;

         // second triangle
         indices[offset + 3] = b;
         indices[offset + 4] = c;
         indices[offset + 5] = d;
      }
   }
}

Display::Display()
{
   clearScreen();
}

bool Display::getValue(int x, int y)
{
   return display[x][y];
}
void Display::setValue(int x, int y, bool value)
{
   display[x][y] = value;
}

void Display::clearScreen()
{
   for (int y = 0; y < SCREEN_HEIGHT; y++)
   {
      for (int x = 0; x < SCREEN_WIDTH; x++)
      {
         display[x][y] = false;
      }
   }
}

void Display::drawScreen()
{
   for (int y = 0; y < SCREEN_HEIGHT; y++)
   {
      for (int x = 0; x < SCREEN_WIDTH; x++)
      {
         if ((y * SCREEN_WIDTH + x) % 2)
            display[x][y] = true;
         if (display[x][y])
         {
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)((y * SCREEN_WIDTH + x) * 6 * sizeof(unsigned int))); // draw 2 triangles from EBO
         }
      }
   }
}