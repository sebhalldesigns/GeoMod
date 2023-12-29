#ifndef GKERNEL_H
#define GKERNEL_H

#include "gKernelServer.h"
#include "gKernelFunction.h"
#include "../Debug/gLog.h"

#include <cstdint>
#include <chrono>
#include <csignal>
#include <atomic>
#include <vector>
#include <functional>
#include <string>


// Global flag used to exit the program loop
static std::atomic<bool> exitFlag(false);

//  gKernel is a class that provides the entry-point
//  to GeoMod and it's modular system.

class gKernel {

    // Signal handler for SIGINT
    static void signalHandler(int signal) {
        if (signal == SIGINT) {
            gLog::Info("Ctrl+C detected, shutting down...");
            exitFlag.store(true);
        }
    }

public:
    static std::vector<gKernelFunction> startupModuleFuctions;

    static int KernelMain(int argc, char *argv[]) {
        // Set the signal handler for SIGINT
        std::signal(SIGINT, signalHandler);
        
        uint64_t elapsedTime = 0;

        for (gKernelFunction function : startupModuleFuctions) {
            // init module passing an api handler as the 1 parameter
            gKernelGenericParameter genericParameter =  { "gKernelFunction", gKernelFunction_t, &APIHandler };
            function("InitModule", InitModule_f, 1, genericParameter);
        }

        if (!gKernelServer::Init("C:\\GeoMod\\server.socket", 2023)) {
            return -1;
        }

        // Main loop of your application
        while (!exitFlag.load()) {
            std::optional<std::string> response = gKernelServer::Poll();
            if (response.has_value()) {
                gLog::Info("Server recieved '%s'", response.value().c_str());
            }
        }

        gKernelServer::Close();
        gLog::Info("Shutdown complete. GeoMod out.");

        return 0;
    };

    static void APIHandler(const char* functionRaw, size_t functionHash, size_t numParameters, ...) { 
        switch (functionHash) {
        case gLog_Info_f: {
            va_list args;
            va_start(args, numParameters);

            for (int i = 0; i < numParameters; ++i) {
                gKernelGenericParameter genericParameter = va_arg(args, gKernelGenericParameter);
                switch (genericParameter.typeHashed) {
                    case std_string_t: {
                        gLog::Info("%s", static_cast<std::string*>(genericParameter.ptr)->c_str());
                        
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

};

std::vector<gKernelFunction> gKernel::startupModuleFuctions;

#endif // GKERNEL_H
