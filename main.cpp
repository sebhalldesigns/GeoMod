#include "GeoMod/Debug/gLog.h"
#include "GeoMod/Kernel/gKernel.h"

int main(int argc, char *argv[]) {

    gLog::Info("Starting GeoMod...");

    return gKernel::KernelMain(argc, argv);
}