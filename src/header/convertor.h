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

#define PARAM(param) ("--" #param)

#define PARAM_DEBUG PARAM(debug)
#define PARAM_LOG_NOT_CACHE_HEX PARAM(log_not_cache_hex)
#define PARAM_LOG_NOT_COMPILER PARAM(log_not_compiler)
#define PARAM_NOT_PACK_LOG PARAM(not_pack_log)
#define PARAM_NO_AUTO_DETECTED_COMPILER PARAM(no_auto_detected_compiler)

struct global_settings 
{
    char input[255];
    char output[255];
    char launch_method[255];
    char launch_class[255];
    
    char gnu_compiler[255];

    char template_jar_to_dll[255];
    char template_dll_to_exe[255];
    char log_dir[255];

    int exit_code;
    bool debug;

    int argc;
    char** argv;
};

struct global_settings parse_global_settings(const char* path);
void global_settings_set_args(struct global_settings* _this, int argc, char** argv);

extern struct global_settings global;

struct buildItem 
{
  const char* input;
  const char* output;

  uint8_t type_build;
};

struct buildItem create_build_item(const char* input, const char* output, uint8_t type);
void build_jar_to_dll(struct buildItem* const _this);
void build_dll_to_exe(struct buildItem* const _this);

void build_for_item(struct buildItem* const _this);

struct Logger* GetConfigLogger();
struct Logger* GetDebugLogger();
struct Logger* GetMainLogger();

#define ConfigLogger GetConfigLogger()
#define DebugLogger GetDebugLogger()
#define MainLogger GetMainLogger()

void pack_log_folder();

void compile_gnu(const char* arguments);
void compile_gnu_cs2(const char* compiler, const char* args);
void compile_gnu_cs3(const char* line);
void output_log(const char* filename, const char* format, ...);
void output_copy(const char* filename, const char* from);

#define has_arg(searched) has_argument(global.argc, global.argv, searched)
#endif
