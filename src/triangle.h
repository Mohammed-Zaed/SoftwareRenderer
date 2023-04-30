#ifndef _TRIANGLE_H_
#define _TRIANGLE_H_

#include <stdint.h>
#include "texture.h"
#include "vector.h"


typedef struct {
    uint32_t a;
    uint32_t b;
    uint32_t c;
    
    tex2_t uva;
    tex2_t uvb;
    tex2_t uvc;
    uint32_t color;
} face_t;

typedef struct {
    vec4_t points[3];
    tex2_t texCoord[3];
    uint32_t color;
} traingle_t;

void drawFilledTriangle(
        int32_t x0, int32_t y0, float z0, float w0,
        int32_t x1, int32_t y1, float z1, float w1,
        int32_t x2, int32_t y2, float z2, float w2,
        uint32_t color 
);
void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color);
void drawTexturedTriangle(
        int32_t x0, int32_t y0, float z0, float w0, float u0, float v0,
        int32_t x1, int32_t y1, float z1, float w1, float u1, float v1,
        int32_t x2, int32_t y2, float z2, float w2, float u2, float v2,
        uint32_t* texture
);
vec3_t baryCentricWeights(vec2_t p, vec2_t a, vec2_t b, vec2_t c);
void drawTexel(vec2_t p, vec4_t a, tex2_t ta, vec4_t b, tex2_t tb, vec4_t c, tex2_t tc, uint32_t* texture);
void drawTrianglePixel(vec2_t p, vec4_t a, vec4_t b, vec4_t c, uint32_t color);
#endif