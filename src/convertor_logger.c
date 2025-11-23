#include "header/convertor.h"
#include "header/files.h"
#include <stdarg.h>

struct Logger* GetConfigLogger()
{
    static struct Logger logger = { 0 };
    logger = create_logger("Config");
    return &logger;
}

struct Logger* GetDebugLogger()
{
    static struct Logger logger = { 0 };
    logger = create_logger("Debug");
    logger.printf = global.debug;

    COPY_STR(logger.start_prefix, TERMINAL_FG_BRIGHT_BLACK);
    COPY_STR(logger.end_prefix, TERMINAL_RESET);

    return &logger;
}

struct Logger* GetMainLogger()
{
    static struct Logger logger = { 0 };
    logger = create_logger("Main");
    return &logger;
}

void output_log(const char* filename, const char* format, ...)
{
    if (!global.log_dir || is_empty(global.log_dir) ||
        ExistDirectoryA(global.log_dir) != 0) {
        return;
    }

    const char* path = ADD_STR3(global.log_dir, "\\", filename);
    va_list list;

    va_start(list, format);
        write_file_va(path, format, list);
    va_end(list);
}

void output_copy(const char* filename, const char* from)
{
    if (!global.log_dir || is_empty(global.log_dir) ||
        ExistDirectoryA(global.log_dir) != 0) {
        return;
    }

    const char* path = ADD_STR3(global.log_dir, "\\", filename);
    DEBUG_ASSERT(file_copy(from, path) == 0);
}


