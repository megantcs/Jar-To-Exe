#include "header/convertor.h"

struct buildItem create_build_item(const char* input, const char* output, uint8_t type)
{
    return (struct buildItem) {
        .input = input,
        .output = output,
        .type_build = type
    };
}