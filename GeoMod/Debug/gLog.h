#ifndef GLOG_H
#define GLOG_H

#include <cstdio>
#include <cstdarg>

const char* BRIGHT_RED = "\033[91m";
const char* RED = "\033[31m";
const char* GREEN = "\033[32m";
const char* YELLOW = "\033[33m";
const char* BLUE = "\033[34m";
const char* RESET = "\033[0m";

class gLog {
public:

    static void Info(const char* format, ...) {
        printf("[%sINFO%s] ", BLUE, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

    static void Warning(const char* format, ...) {
        printf("[%sWARNING%s] ", YELLOW, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

    static void Error(const char* format, ...) {
        printf("[%sERROR%s] ", RED, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

    static void Critical(const char* format, ...) {
        printf("[%sCRITICAL%s] ", BRIGHT_RED, RESET);

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        printf("\n");
    }

};



#endif // GLOG_H