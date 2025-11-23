#ifndef FILES_H
#define FILES_H

char* get_str_files_in_dir(const char* dir, const char* pattern);
int appendToFile(const char* filename, const char* text);
int exist_directory(const char* path);

#define ExistDirectoryA(path) exist_directory(path)

#endif