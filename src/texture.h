#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <stdint.h>

typedef struct {
    float u;
    float v;
} tex2_t;

extern uint32_t textureWidth;
extern uint32_t textureHeight;

extern const uint8_t RED_BRICK_TEXTUE[];
extern uint32_t* meshTexture;

#endif