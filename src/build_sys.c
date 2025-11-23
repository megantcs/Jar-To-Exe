#include "header/build_sys.h"
#include <stdio.h>
#include <string.h>
#include "header/win32.h"
#include "header/convertor.h"
#include <malloc.h>

#define NONE ""

BuildSys* BUILD_SYS_METHOD(ctor)()
{
    BuildSys* _ = (BuildSys*)malloc(sizeof(BuildSys));
    if (!_) return NULL;

    memset(_, 0, sizeof(BuildSys));  

    _->custom_args = NULL;
    _->files = NULL;

    COPY_STR(_->compiler, NONE);

    return _;
}

void BUILD_SYS_METHOD(dtor)(BuildSys* _this)
{
    DEBUG_ASSERT(_this);

    if (_this) {
        free(_this->custom_args);
        free(_this->files);
        free(_this);
    }
}

void BUILD_SYS_METHOD(add_file)(BuildSys* _this, const char* file)
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(file);

    if (!_this || !file) return;

    if (_this->files == NULL) {
        _this->files = malloc(strlen(file) + 1);
        if (_this->files) {
            strcpy(_this->files, file);
        }
    }
    else {
        size_t new_size = strlen(_this->files) + strlen(file) + 2; 
        char* new_files = realloc(_this->files, new_size);
        if (new_files) {
            strcat(new_files, " ");
            strcat(new_files, file);
            _this->files = new_files;
        }
    }
}

void BUILD_SYS_METHOD(add_arg)(BuildSys* _this, const char* arg)
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(arg);

    if (!_this || !arg) return;

    if (_this->custom_args == NULL) {
        _this->custom_args = malloc(strlen(arg) + 1);
        if (_this->custom_args) {
            strcpy(_this->custom_args, arg);
        }
    }
    else {
        size_t new_size = strlen(_this->custom_args) + strlen(arg) + 2;
        char* new_args = realloc(_this->custom_args, new_size);
        if (new_args) {
            strcat(new_args, " ");
            strcat(new_args, arg);
            _this->custom_args = new_args;
        }
    }
}

void BUILD_SYS_METHOD(set_output) (BuildSys* _this, const char* output)
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(output);

    if (_this && output) {
        strncpy(_this->output, output, sizeof(_this->output) - 1);
        _this->output[sizeof(_this->output) - 1] = '\0';
    }
}

void BUILD_SYS_METHOD(set_compiler) (BuildSys* _this, const char* compiler)
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(compiler);

    if(_this && compiler) {
        strncpy(_this->compiler, compiler, sizeof(_this->compiler) - 1);
        _this->compiler[sizeof(_this->compiler) - 1] = '\0';
    }
}

void BUILD_SYS_METHOD(clear_files) (BuildSys* _this)
{
    DEBUG_ASSERT(_this);

    if (_this && _this->files) {
        free(_this->files);
        _this->files = NULL;
    }
}

const char* BUILD_SYS_METHOD(first_detect_compiler)()
{
    const char* compilers[] = {
        "g++",
        "clang++",
        NULL  
    };

    for (int i = 0; compilers[i] != NULL; i++) {  
        if (is_cmd_path(compilers[i])) {
            log(DebugLogger, "auto-detected compiler: %s", compilers[i]);
            return compilers[i];
        }
    }

    log(DebugLogger, "no compiler detected in PATH");
    return "g++";  
}

void BUILD_SYS_METHOD(build)(BuildSys* _this) 
{
    DEBUG_ASSERT(_this);

    if (_this) {
        compile_gnu_cs3(build_sys_get_cmd(_this));
    }
}

const char* BUILD_SYS_METHOD(get_cmd)(BuildSys* _this) 
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(_this->compiler  || _this->compiler == NONE);

    if (!_this) return NULL;

    static char cmd_line[1024];  

    const char* base_flags = "-std=c++20 -static -static-libgcc -static-libstdc++ -fpermissive";
    const char* output_name = _this->output[0] ? _this->output : "output.exe";

    snprintf(cmd_line, sizeof(cmd_line),
        "%s %s %s -o \"%s\" %s",
        _this->compiler[0] ? _this->compiler : "g++",  
        base_flags,
        _this->custom_args ? _this->custom_args : "",  
        output_name,
        _this->files ? _this->files : "");             

    return cmd_line;
}

