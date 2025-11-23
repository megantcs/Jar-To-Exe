#include "header/hex_tools.h"
#include "header/timer.h"
#include "header/convertor.h"

static void hex_dump_v1(FILE* output, const unsigned char* data, size_t data_size)
{
    const int bytesPerLine = 12;
    fprintf(output, "#define __SIZE__ 0x%zX\n\n\n", data_size);
    fprintf(output, "static unsigned char rawData[__SIZE__] = {\n");

    for (size_t i = 0; i < data_size; i += bytesPerLine) {
        fprintf(output, "\t");
        for (size_t j = 0; j < bytesPerLine && (i + j) < data_size; j++) {
            fprintf(output, "0x%02X", data[i + j]);
            if ((i + j + 1) < data_size) {
                fprintf(output, ", ");
            }
        }
        fprintf(output, "\n");
    }
    fprintf(output, "};\n");
}

long get_file_size(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) return -1;

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fclose(file);

    return size;
}

int file_to_hex_dump(const char* input_file, const char* output_file)
{
    struct timer* timer = ctor_timer();
    start_timer(timer);

    FILE* input = NULL;
    FILE* output = NULL;
    unsigned char* buffer = NULL;
    int return_code = 1;

    long file_size = get_file_size(input_file);
    if (file_size < 0) {
        return 2;
    }

    input = fopen(input_file, "rb");
    if (!input) {
        return 2;
    }

    output = fopen(output_file, "w");
    if (!output) {
        fclose(input);
        return 2;
    }

    buffer = (unsigned char*)malloc(file_size);
    if (!buffer) {
        return_code = 2;
        goto cleanup;
    }

    size_t bytes_read = fread(buffer, 1, file_size, input);
    if (bytes_read != (size_t)file_size) {
        return_code = 2;
        goto cleanup;
    }

    fprintf(output, "/* StartOffset(h): 00000000, EndOffset(h): %08zX, Size(h): %08zX */\n\n",
        (size_t)(file_size - 1), (size_t)file_size);

    hex_dump_v1(output, buffer, file_size);
    return_code = 0;

cleanup:
    if (buffer) free(buffer);
    if (input) fclose(input);
    if (output) fclose(output);

    double ms = end_timer_ms(timer);
    dtor_timer(timer);

    double second = ms / 1000;
    log(DebugLogger, "~ execute hex dumper: %.2f second", second);
    
    if (has_arg(PARAM_LOG_NOT_CACHE_HEX) == 0)
        output_copy("last-cache-hex.h", output_file);

    return return_code;
}

int buffer_to_hex_dump(FILE* output, const unsigned char* data, size_t data_size)
{
    if (!output || !data) {
        return 1;
    }

    fprintf(output, "/* StartOffset(h): 00000000, EndOffset(h): %08zX, Size(h): %08zX */\n\n",
        (size_t)(data_size - 1), (size_t)data_size);

    hex_dump_v1(output, data, data_size);

    return 0;
}