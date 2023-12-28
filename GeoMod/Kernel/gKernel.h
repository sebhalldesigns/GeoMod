#ifndef GKERNEL_H
#define GKERNEL_H

#include "gKernelServer.h"
#include "../Debug/gLog.h"

#include <cstdint>
#include <chrono>
#include <csignal>
#include <atomic>

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
    static int KernelMain(int argc, char *argv[]) {
        // Set the signal handler for SIGINT
        std::signal(SIGINT, signalHandler);

        uint64_t elapsedTime = 0;

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

};


#endif // GKERNEL_H
