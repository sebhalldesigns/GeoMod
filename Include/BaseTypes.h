#ifndef BASETYPES_H
#define BASETYPES_H

#include <cstdint>


// these types are defined so that they can 
// be simply and effectively upgraded to reduce limits.
// For example, the 64 bit integer has a maximum value
// in the region of 1.8x10^19 which should be large enough
// for now, but could become a limiting factor in the future.

typedef uint64_t gId;
typedef double gFloat;

#endif // BASETYPES_H