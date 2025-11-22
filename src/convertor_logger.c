#include "header/convertor.h"

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
    return &logger;
}

struct Logger* GetMainLogger()
{
    static struct Logger logger = { 0 };
    logger = create_logger("Main");
    return &logger;
}


