#include "header/convertor.h"
#include "header/logger.h"
#include "header/win32.h"
#include "header/files.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

struct global_settings global;

struct global_settings parse_global_settings(const char* path)
{
    FILE* file = fopen(path, "r");
    if (file == nullptr)
    {
        log(ConfigLogger, "file not found: %s", path);
        exit(3);
    }

    struct global_settings settings = { 0 };
    settings.debug = false;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\n")] = 0;

        char key[256], value[256];
        if (sscanf(line, "%99[^=]=%99s", key, value) == 2)
        {
            if (strcmp(key, "input") == 0) strcpy(settings.input, value);
            if (strcmp(key, "output") == 0) strcpy(settings.output, value);
            if (strcmp(key, "class") == 0) strcpy(settings.launch_method, value);
            if (strcmp(key, "method") == 0) strcpy(settings.launch_class, value);
            if (strcmp(key, "compiler") == 0) strcpy(settings.gnu_compiler, value);
            if (strcmp(key, "template_dll_to_exe") == 0) strcpy(settings.template_dll_to_exe, value);
            if (strcmp(key, "template_jar_to_dll") == 0) strcpy(settings.template_jar_to_dll, value);
            if (strcmp(key, "log_dir") == 0) strcpy(settings.log_dir, value);
        }
    }
    fclose(file);

    return settings;
}

void global_settings_set_args(struct global_settings* _this, PROGRAM_ARGUMENTS)
{
    DEBUG_ASSERT(_this);
    DEBUG_ASSERT(argv);

    _this->argc = argc;
    _this->argv = argv;
}

void pack_log_folder()
{
    if (has_arg(PARAM_NOT_PACK_LOG) == true) 
        return; 
    
    if (!global.log_dir || is_empty(global.log_dir)) {
        return;  
    }

    if (ExistDirectoryA(global.log_dir) != 0) {
        char mkdir_cmd[255];
        snprintf(mkdir_cmd, sizeof(mkdir_cmd), "mkdir %s", global.log_dir);
        popen(mkdir_cmd);
    }


    char p[255];
    snprintf(p, sizeof(p), "%s\\%s", global.log_dir, temp_is_time(".zip"));

    log(DebugLogger, "pack old log...");
    zip_archive(global.log_dir, p);
}