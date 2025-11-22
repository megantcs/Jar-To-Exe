#ifndef CONVERTOR_H
#define CONVERTOR_H

#include <stdint.h>
#include "logger.h"
#include "utils.h"

#define DLL_TO_EXE 0
#define JAR_TO_DLL 1
#define JAR_TO_EXE 2

#define bool int
#define true 1
#define false 0
#define nullptr 0

struct global_settings 
{
    char input[255];
    char output[255];
    char launch_method[255];
    char launch_class[255];
    char gnu_compiler[255];
};

struct global_settings parse_global_settings(const char* path);
extern struct global_settings global;

struct buildItem 
{
  const char* input;
  const char* output;

  uint8_t type_build;
};

struct buildItem create_build_item(const char* input, const char* output, uint8_t type);
void build_jar_to_dll(struct buildItem* const _this);

struct Logger* GetConfigLogger();
struct Logger* GetDebugLogger();
struct Logger* GetMainLogger();

#define ConfigLogger GetConfigLogger()
#define DebugLogger GetDebugLogger()
#define MainLogger GetMainLogger()


#endif
