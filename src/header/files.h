#ifndef FILES_H
#define FILES_H
#include <stdio.h>

char* get_str_files_in_dir(const char* dir, const char* pattern);
int appendToFile(const char* filename, const char* text);
int exist_directory(const char* path);

int write_file(const char* path, const char* format, ...);
const char* read_file(const char* path);
int write_file_va(const char* path, const char* format, va_list args);
int file_copy(const char* from, const char* to);
const char* get_file_name(const char* path);

#define ExistDirectoryA(path) exist_directory(path)

#endif