#include "GeoMod/Debug/gLog.h"
#include "GeoMod/Kernel/gKernel.h"
#include "GeoMod/Kernel/gKernelFunction.h"
#include <stdio.h>

gKernelFunction kernelAPI;

void MyAPIHandler(const char* functionRaw, size_t functionHash, size_t numParameters, ...) {
    switch (functionHash) {
        case InitModule_f: {
            va_list args;
            va_start(args, numParameters);

            for (int i = 0; i < numParameters; ++i) {
                gKernelGenericParameter genericParameter = va_arg(args, gKernelGenericParameter);
                switch (genericParameter.typeHashed) {
                    case gKernelFunction_t: {
                        kernelAPI = static_cast<gKernelFunction>(genericParameter.ptr);
                        gKernelGenericParameter genericParameter;
                        std::string* myString = new std::string("hello kernel interface!");
                        genericParameter.typeHashed = std_string_t;
                        genericParameter.typeRaw = "std::string";
                        genericParameter.ptr = myString;
                        kernelAPI("gLog::Info", gLog_Info_f, 1, genericParameter);
                        delete myString;
                        break;
                    }

                    default: {
                        gLog::Error("Escaping API call due to unknown parameter type %s", genericParameter.typeRaw);
                        return;
                    }
                }
            }

            va_end(args);
            return;
        }
    }
}

int main(int argc, char *argv[]) {

    gLog::Info("Starting GeoMod...");

    gKernel::startupModuleFuctions.push_back(&MyAPIHandler);

    return gKernel::KernelMain(argc, argv);
}