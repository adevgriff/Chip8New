#ifndef CONSTANTS_H
#define CONSTANTS_H

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define PIXEL_SIZE 15
#define SCREEN_VERTICES (((SCREEN_WIDTH + 1) * (SCREEN_HEIGHT + 1)) * 3)
#define TOTAL_INDICES (6 * SCREEN_WIDTH * SCREEN_HEIGHT) // 3 vertexes per triangle 2 triangles per square so multiply by 6

#define RAMSIZE_BYTES 4096
#define START_OF_FONT 80
#define END_OF_FONT 159
#define START_OF_PROGRAM 512

#endif