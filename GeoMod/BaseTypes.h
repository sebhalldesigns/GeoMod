#ifndef BASETYPES_H
#define BASETYPES_H


#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>

// these types are defined so that they can 
// be simply and effectively upgraded to reduce limits.
// For example, the 64 bit integer has a maximum value
// in the region of 1.8x10^19 which should be large enough
// for now, but could become a limiting factor in the future.

typedef uint64_t gId;
typedef uint32_t gIterator;
typedef double gFloat;
typedef int64_t gInt;

// here are some constants
static const double TAU = 2.0*M_PI;

struct gSize {
    gFloat width;
    gFloat height;
    gFloat depth;
};

struct gOffset {
    gFloat x;
    gFloat y;
    gFloat z;
};

struct gMatrix4x4 {
    gFloat data[16];

    static gMatrix4x4 Identity() {
        return {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };
    }
};

#endif // BASETYPES_H