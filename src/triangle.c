#include "display.h"
#include "triangle.h"

static void int32Swap(int32_t* a, int32_t* b) {
    int32_t temp = *a;
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