#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "SDL2/SDL.h"

uint8_t initWindow(void);
void drawGrid(uint32_t deltaX, uint32_t deltaY, uint32_t color);
void clearColorBuffer(uint32_t color);
void renderColorBuffer(void);
void destroyWindow(void);
void drawFillRectangle(uint32_t x, uint32_t y, uint32_t l1, uint32_t l2, uint32_t color);
void drawPixel(uint32_t x, uint32_t y, uint32_t color);

#endif