#include "display.h"
#include "triangle.h"

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

static void drawFillFlatBottomTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    // Start from the top
    // Calculate two slopes
    float deltaX1PerY1 = (float)(x1 - x0) / (float)(y1 - y0);
    float deltaX2PerY2 = (float)(x2 - x0) / (float)(y2 - y0);
    
    // Start xStart and xEnd from the top vertex
    float xStart = x0;
    float xEnd = x0;

    // Loop all the scan lines from top to bottom
    for (int32_t y = y0; y <= y2; ++y) {
        drawLine(xStart, y, xEnd, y, color);
        xStart += deltaX1PerY1;
        xEnd += deltaX2PerY2;
    }
}

static void drawFillFlatTopTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    // Start from the bottom
    // Calculate two slopes
    float deltaX0PerY0 = (float)(x2 - x0) / (float)(y2 - y0);
    float deltaX1PerY1 = (float)(x2 - x1) / (float)(y2 - y1);
    
    // Start xStart and xEnd from the bottom vertex
    float xStart = x2;
    float xEnd = x2;

    // Loop all the scan lines from bottom to top
    for (int32_t y = y2; y >= y0; --y) {
        drawLine(xStart, y, xEnd, y, color);
        xStart -= deltaX0PerY0;
        xEnd -= deltaX1PerY1;
    }


}

void drawFilledTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    // Sort vertices by y-coordinates ascending order (y0 < y1 < y2)
    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
    }
    
    if (y1 > y2) {
        int32Swap(&y1, &y2);
        int32Swap(&x1, &x2);
    }

    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
    }
    
    if (y1 == y2) {
        drawFillFlatBottomTriangle(x0, y0, x1, y1, x2, y2, color);
    } else if (y0 == y1) {
        drawFillFlatTopTriangle(x0, y0, x1, y1, x2, y2, color);
    } else {
        // Split the triangle Flat bottom and Flat Top
        // Calculate the new vertex (Mx. My) using tirangle similarity
        int32_t My = y1;
        int32_t Mx = ((float)((x2 - x0) * (y1 - y0)) / (float)(y2 - y0)) + x0;
        drawFillFlatBottomTriangle(x0, y0, x1, y1, Mx, My, color);
        drawFillFlatTopTriangle(x1, y1, Mx, My, x2, y2, color);
    }
}

void drawTriangle(int32_t x0, int32_t y0, int32_t x1, int32_t y1, int32_t x2, int32_t y2, uint32_t color) {
    drawLine(x0, y0, x1, y1, color);
    drawLine(x1, y1, x2, y2, color);
    drawLine(x2, y2, x0, y0, color);
}

void drawTexturedTriangle(
        int32_t x0, int32_t y0, float u0, float v0,
        int32_t x1, int32_t y1, float u1, float v1,
        int32_t x2, int32_t y2, float u2, float v2,
        uint32_t* texture
) {
    // Sort vertices by y-coordinates ascending order (y0 < y1 < y2)
    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }
    
    if (y1 > y2) {
        int32Swap(&y1, &y2);
        int32Swap(&x1, &x2);
        floatSwap(&u1, &u2);
        floatSwap(&v1, &v2);
    }

    if (y0 > y1) {
        int32Swap(&y0, &y1);
        int32Swap(&x0, &x1);
        floatSwap(&u0, &u1);
        floatSwap(&v0, &v1);
    }
    
    vec2_t a = {x0, y0};
    vec2_t b = {x1, y1};
    vec2_t c = {x2, y2};

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
        
        for (int32_t x = xStart; x < xEnd; ++x)  {
            vec2_t p = {x, y};
            drawTexel(p, a, ta, b, tb, c, tc, meshTexture); 
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
            drawTexel(p, a, ta, b, tb, c, tc, meshTexture); 
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

void drawTexel(vec2_t p, vec2_t a, tex2_t ta, vec2_t b, tex2_t tb, vec2_t c, tex2_t tc, uint32_t* texture) {

    vec3_t weigths = baryCentricWeights(p, a, b, c);

    const float alpha = weigths.x;
    const float beta = weigths.y;
    const float gamma = weigths.z;
    
    const float interpolatedU = ta.u * alpha + tb.u * beta + tc.u * gamma;
    const float interpolatedY = ta.v * alpha + tb.v * beta + tc.v * gamma;
    
    const int32_t tx = abs((int32_t)(interpolatedU * textureWidth));
    const int32_t ty = abs((int32_t)(interpolatedY * textureHeight));
    
    drawPixel(p.x, p.y, texture[(ty * textureWidth) + tx]);
}
