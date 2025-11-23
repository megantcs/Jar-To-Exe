#define _CRT_SECURE_NO_WARNINGS
#include "header/convertor.h"
#include "header/win32.h"

#include <stdio.h>
#include <windows.h>

int is_error_gnu(const char* t) 
{
    DEBUG_ASSERT(t);

    if (find_str(t, "fatal error:") != InvalidPos) return 1;
    if (find_str(t, "compilation terminated.") != InvalidPos) return 1;

    return 0;
}

char* system_str(const char* command) {
    return popen(command);
}

void compile_gnu(const char* arguments)
{
    DEBUG_ASSERT(arguments);

    COPY_OR_NULL(global.gnu_compiler, "g++");
    compile_gnu_cs2(global.gnu_compiler, arguments);
}

void compile_gnu_cs2(const char* compiler, const char* arguments)
{
    DEBUG_ASSERT(compiler);
    DEBUG_ASSERT(arguments);
        
    char buf[512];
    snprintf(buf, sizeof(buf), "%s -fpermissive -static-libgcc -static-libstdc++ -static -std=c++20 %s", compiler, arguments);
    compile_gnu_cs3(buf);
}

void compile_gnu_cs3(const char* line)
{
    DEBUG_ASSERT(line);

    log(DebugLogger, "Execute: %s", line);
    char* output = system_str(line);  

    if (output) {
        if (is_error_gnu(output)) {
            global.exit_code = -7;
            log(MainLogger, TERMINAL_BG_RED "--- ERROR ---\n%s\n" TERMINAL_RESET, output);
        }

        if (has_arg(PARAM_LOG_NOT_COMPILER) == FALSE)
            output_log("last-compile.log", "Compile line=%s\n\nOutput:%s\n", line, output);

        free(output);  
    }
}

