/*
   code by https://github.com/megant0/JavaTransformer/blob/master/src/JavaTransformer.Core.HexDumper/main.cpp
*/
#ifndef HEX_TOOLS_H
#define HEX_TOOLS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int file_to_hex_dump(const char* input_file, const char* output_file);
int buffer_to_hex_dump(FILE* output, const unsigned char* data, size_t data_size);
long get_file_size(const char* filename);

#endif