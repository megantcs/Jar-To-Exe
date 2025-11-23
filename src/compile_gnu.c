#include "header/convertor.h"
#include <stdio.h>
#include <stdlib.h>

#include <Windows.h>

void compile_gnu(const char* arguments)
{
    char buf[512];

    // пока как то так
    COPY_OR_NULL(global.gnu_compiler, "g++");

    snprintf(buf, sizeof(buf), "\"%s\" -fpermissive -static-libgcc -static-libstdc++ -static -std=c++20 %s", global.gnu_compiler, arguments);
    log(DebugLogger, "Execute: %s", buf);

    system(buf);
}