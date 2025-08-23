#pragma once

#include <stdint.h>



struct Point_ {
    float X;
    float Y;
};
typedef struct Point_ Point;

struct Rectangle_ {
    int32_t X;
    int32_t Y;
    int32_t Width;
    int32_t Height;
};
typedef struct Rectangle_ Rectangle;