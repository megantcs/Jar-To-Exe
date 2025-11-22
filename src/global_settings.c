#include "header/convertor.h"
#include "header/logger.h"

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
        }
    }
    fclose(file);

    return settings;
}

