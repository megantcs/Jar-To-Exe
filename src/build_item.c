#include "header/convertor.h"
#include <stdio.h>

struct buildItem create_build_item(const char* input, const char* output, uint8_t type)
{
    return (struct buildItem) {
        .input = input,
        .output = output,
        .type_build = type
    };
}

void build_for_item(struct buildItem* const _this)
{
    switch (_this->type_build) {
        case JAR_TO_DLL: { build_jar_to_dll(_this); break; }
        case DLL_TO_EXE: { build_dll_to_exe(_this); break; }
        default: 
        {
            log(MainLogger, "Unknown type build %d", _this->type_build);
            global.exit_code = -1;
        }
    }
}
