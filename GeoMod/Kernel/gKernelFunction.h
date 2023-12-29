#ifndef GKERNELFUNCTION_H
#define GKERNELFUNCTION_H

#include <stdint.h>
#include <stdarg.h>
#include <functional>
#include <string>

struct gKernelGenericParameter {
    const char* typeRaw;
    size_t typeHashed;
    void* ptr;
};

typedef void (*gKernelFunction)(const char* functionRaw, size_t functionHash, size_t numParameters, ...);

// the memory interface is as follows:
// all variables (except for the numparameters) are passsed as pointers
// the caller allocates the pointers to the parameters, ensuring they are safe to 
// read and write where required.
// The callee then reads and writes the values as defined.
// No allocation is performed by the callee on variables passed to/from the function.
// the caller deallocates variables where appropriate afterwards.

// IMPORTANT: the function should only return on receipt of the callee.
// i.e it should be safe to allocate memory, call the function and then immediately deallocate memory.

constexpr size_t HashFromString(const char* str) {
    constexpr size_t prime = 263;  // Alternative prime number
    constexpr size_t modulus = 2305843009213693951ULL; // Largest 64-bit prime

    size_t total = 0;
    size_t current_multiplier = 1;

    for (const char* it = str; *it != '\0'; ++it) {
        total = (total + static_cast<std::size_t>(*it) * current_multiplier) % modulus;
        current_multiplier = (current_multiplier * prime) % modulus;
    }

    return total;
}

const size_t InitModule_f = HashFromString("InitModule");
const size_t gLog_Info_f = HashFromString("gLog::Info");
const size_t std_string_t = HashFromString("std::string");
const size_t gKernelFunction_t = HashFromString("gKernelFunction");


#endif // GKERNELFUNCTION_H