#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <stdint.h>
#include "vector.h"


typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    uint32_t color;
} face_t;

typedef struct {
    vec2_t points[3];
    uint32_t color;
} traingle_t;

void drawFilledTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);



#endif