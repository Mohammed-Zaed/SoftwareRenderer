#include "display.h"
#include "triangle.h"

extern uint32_t winWidth;
extern float* zBuffer;

static void int32Swap(int32_t* a, int32_t* b) {
    int32_t temp = *a;
    *a = *b;
    *b = temp;
}

static void floatSwap(float* a, float *b) {
    float temp = *a;
    *a = *b;
    *b = temp;
}

void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void drawFilledTriangle(
        int32_t x0, int32_t y0, float z0, float w0,
        int32_t x1, int32_t y1, float z1, float w1,
        int32_t x2, int32_t y2, float z2, float w2,
        uint32_t color 
) {
    // Sort vertices by y-coordinates ascending order (y0 < y1 < y2)
    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
    }
    
    if (y1 > y2) {
        int32Swap(&y1, &y2);
        int32Swap(&x1, &x2);
        floatSwap(&z1, &z2);
        floatSwap(&w1, &w2);
    }

    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
    }
    
    vec4_t a = {x0, y0, z0, w0};
    vec4_t b = {x1, y1, z1, w1};
    vec4_t c = {x2, y2, z2, w2};
    
    //Draw the flat-bottom of Triangle
    float slope1 = ((y1 - y0) != 0) ? ((float)(x1 - x0) / abs(y1 - y0)) : 0.0F;
    float slope2 = ((y2 - y0) != 0) ? ((float)(x2 - x0) / abs(y2 - y0)) : 0.0F;
    
    for (int32_t y = y0; y < y1; ++y) {
        int32_t xStart = x1 + (y - y1) * slope1;
        int32_t xEnd = x0 + (y - y0) * slope2;
        
        if (xEnd < xStart) {
            int32Swap(&xStart, &xEnd);
        }
        
        for (int32_t x = xStart; x < xEnd; ++x) {
            vec2_t p = {x, y};
            drawTrianglePixel(p, a, b, c, color); 
        }
    }

    //Draw the flat-top of Triangle
    slope1 = ((y2 - y1) != 0) ? ((float)(x2 - x1) / abs(y2 - y1)) : 0.0F;
    slope2 = ((y2 - y0) != 0) ? ((float)(x2 - x0) / abs(y2 - y0)) : 0.0F;
    
    for (int32_t y = y1; y < y2; ++y) {
        int32_t xStart = x1 + (y - y1) * slope1;
        int32_t xEnd = x0 + (y - y0) * slope2;
        
        if (xEnd < xStart) {
            int32Swap(&xStart, &xEnd);
        }
        
        for (int32_t x = xStart; x < xEnd; ++x)  {
            vec2_t p = {x, y};
            drawTrianglePixel(p, a, b, c, color); 
        }
    }
}

void drawTexturedTriangle(
        int32_t x0, int32_t y0, float z0, float w0, float u0, float v0,
        int32_t x1, int32_t y1, float z1, float w1, float u1, float v1,
        int32_t x2, int32_t y2, float z2, float w2, float u2, float v2,
        uint32_t* texture
) {
    // Sort vertices by y-coordinates ascending order (y0 < y1 < y2)
    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }
    
    if (y1 > y2) {
        int32Swap(&y1, &y2);
        int32Swap(&x1, &x2);
        floatSwap(&z1, &z2);
        floatSwap(&w1, &w2);
        floatSwap(&u1, &u2);
        floatSwap(&v1, &v2);
    }

    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&z0, &z1);
        floatSwap(&w0, &w1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }
    
    vec4_t a = {x0, y0, z0, w0};
    vec4_t b = {x1, y1, z1, w1};
    vec4_t c = {x2, y2, z2, w2};

    v0 = 1 - v0;
    v1 = 1 - v1;
    v2 = 1 - v2;

    tex2_t ta = {u0, v0};
    tex2_t tb = {u1, v1};
    tex2_t tc = {u2, v2};
    
    
    //Draw the flat-bottom of Triangle
    float slope1 = ((y1 - y0) != 0) ? ((float)(x1 - x0) / abs(y1 - y0)) : 0.0F;
    float slope2 = ((y2 - y0) != 0) ? ((float)(x2 - x0) / abs(y2 - y0)) : 0.0F;
    
    for (int32_t y = y0; y < y1; ++y) {
        int32_t xStart = x1 + (y - y1) * slope1;
        int32_t xEnd = x0 + (y - y0) * slope2;
        
        if (xEnd < xStart) {
            int32Swap(&xStart, &xEnd);
        }
        
        for (int32_t x = xStart; x < xEnd; ++x) {
            vec2_t p = {x, y};
            drawTexel(p, a, ta, b, tb, c, tc, texture); 
        }
    }

    //Draw the flat-top of Triangle
    slope1 = ((y2 - y1) != 0) ? ((float)(x2 - x1) / abs(y2 - y1)) : 0.0F;
    slope2 = ((y2 - y0) != 0) ? ((float)(x2 - x0) / abs(y2 - y0)) : 0.0F;
    
    for (int32_t y = y1; y < y2; ++y) {
        int32_t xStart = x1 + (y - y1) * slope1;
        int32_t xEnd = x0 + (y - y0) * slope2;
        
        if (xEnd < xStart) {
            int32Swap(&xStart, &xEnd);
        }
        
        for (int32_t x = xStart; x < xEnd; ++x)  {
            vec2_t p = {x, y};
            drawTexel(p, a, ta, b, tb, c, tc, texture); 
        }
    }
}

vec3_t baryCentricWeights(vec2_t p, vec2_t a, vec2_t b, vec2_t c) {

    vec2_t ac = vec2Sub(c, a);
    vec2_t ab = vec2Sub(b, a);

    vec2_t ap = vec2Sub(p, a);
    vec2_t pc = vec2Sub(c, p);
    vec2_t pb = vec2Sub(b, p);
    
    const float areaOfParallelogramABC = fabs((ac.x * ab.y - ac.y * ab.x));
    const float areaOfParallelogramPBC = fabs((pc.x * pb.y - pc.y * pb.x)); 
    const float areaOfParallelogramAPC = fabs((ac.x * ap.y - ac.y * ap.x));
    
    const float alpha = areaOfParallelogramPBC / areaOfParallelogramABC;
    const float beta = areaOfParallelogramAPC / areaOfParallelogramABC;
    const float gamma = 1.0 - (alpha + beta);
    
    vec3_t weights = {alpha, beta, gamma};
    return weights;
}

void drawTexel(vec2_t p, vec4_t a, tex2_t ta, vec4_t b, tex2_t tb, vec4_t c, tex2_t tc, uint32_t* texture) {

    vec2_t vec2a = vec4ToVec2(a);
    vec2_t vec2b = vec4ToVec2(b);
    vec2_t vec2c = vec4ToVec2(c);
    vec3_t weigths = baryCentricWeights(p, vec2a, vec2b, vec2c);

    const float alpha = weigths.x;
    const float beta = weigths.y;
    const float gamma = weigths.z;

    const float interploated1byW = (1.0 / a.w) * alpha + (1.0 / b.w) * beta + (1.0 / c.w) * gamma;
    const float interpolatedU = ((ta.u / a.w) * alpha + (tb.u / b.w) * beta + (tc.u / c.w) * gamma) / interploated1byW;
    const float interpolatedY = ((ta.v / a.w) * alpha + (tb.v / b.w) * beta + (tc.v / c.w) * gamma) / interploated1byW;

    const int32_t tx = abs((int32_t)(interpolatedU * textureWidth)) % textureWidth;
    const int32_t ty = abs((int32_t)(interpolatedY * textureHeight)) % textureHeight;

    float * zValue = &zBuffer[((int32_t)p.y *  winWidth) + (int32_t)p.x]; 
    float temp1byW = 1.0 - interploated1byW;

    if (temp1byW < *zValue) {
        drawPixel(p.x, p.y, texture[(ty * textureWidth) + tx]);
        *zValue = temp1byW;
    }
}


void drawTrianglePixel(vec2_t p, vec4_t a, vec4_t b, vec4_t c, uint32_t color) {

    vec2_t vec2a = vec4ToVec2(a);
    vec2_t vec2b = vec4ToVec2(b);
    vec2_t vec2c = vec4ToVec2(c);
    vec3_t weigths = baryCentricWeights(p, vec2a, vec2b, vec2c);

    const float alpha = weigths.x;
    const float beta = weigths.y;
    const float gamma = weigths.z;

    const float interploated1byW = (1.0 / a.w) * alpha + (1.0 / b.w) * beta + (1.0 / c.w) * gamma;

    float * zValue = &zBuffer[((int32_t)p.y *  winWidth) + (int32_t)p.x]; 
    float temp1byW = 1.0 - interploated1byW;

    if (temp1byW < *zValue) {
        drawPixel(p.x, p.y, color);
        *zValue = temp1byW;
    }
}