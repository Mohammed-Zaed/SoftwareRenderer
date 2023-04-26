#include "light.h"

uint32_t lightApplyIntensity(uint32_t color, float percentageFactor) {
    percentageFactor = percentageFactor < 0 ? 0 : percentageFactor;
    percentageFactor = percentageFactor > 1 ? 1 : percentageFactor;
    uint32_t a = color & 0xFF000000;
    uint32_t r = (color & 0x00FF0000) * percentageFactor;
    uint32_t g = (color & 0x0000FF00) * percentageFactor;
    uint32_t b = (color & 0x000000FF) * percentageFactor;
    
    const uint32_t result = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
    return result;
}