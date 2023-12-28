#ifndef BASETYPES_H
#define BASETYPES_H


#define _USE_MATH_DEFINES
#include <math.h>
#include <cstdint>
#include <string>

// these types are defined so that they can 
// be simply and effectively upgraded to reduce limits.
// For example, the 64 bit integer has a maximum value
// in the region of 1.8x10^19 which should be large enough
// for now, but could become a limiting factor in the future.

typedef double gFloat;
typedef int64_t gInt;
typedef uint64_t gUint;

typedef uint64_t gId;
typedef uint32_t gIterator;

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

struct gVector3 {
    gFloat data[3];
};

struct gVector4 {
    gFloat data[4];
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

struct gObject {
    virtual std::string ToString() { return "Not Implemented"; };
};

struct gDateTime: gObject {
    // timestamp measuring C#-style 100ns ticks
    gUint rawTimestamp;

    std::string ToString() override {
        return "A Timestamp";
    }
    
    gDuration Duration(gDateTime otherTime) {
        if (otherTime.rawTimestamp > rawTimestamp) {
            return { otherTime.rawTimestamp - rawTimestamp };
        } else {
            return { rawTimestamp - otherTime.rawTimestamp };
        }
    }

};

struct gDuration {

    // duration measuring C#-style 100ns ticks
    gUint rawDuration;

    gUint Ticks() {
        return rawDuration;
    }

    gUint MicroSeconds() {
        return rawDuration / 10;
    }

    gUint MilliSeconds() {
        return rawDuration / 10000;
    }

    gUint Seconds() {
        return rawDuration / 10000000;
    }

    gUint Minutes() {
        return rawDuration / 600000000;
    }

    gUint Hours() {
        return rawDuration / 36000000000;
    }

    gUint Days() {
        return rawDuration / 864000000000;
    }

};

#endif // BASETYPES_H