#include "header/files.h"
#include "header/utils.h"

#include <stdio.h>
#include <string.h>
#include <windows.h>

char* get_str_files_in_dir(const char* dir, const char* pattern)
{
    if (dir == NULL || pattern == NULL) {
        return NULL;
    }

    char* result = malloc(4096);
    if (result == NULL) {
        return NULL;
    }
    result[0] = '\0';

    int first_file = 1;
    char search_path[1024];
    if (strlen(dir) + 3 > sizeof(search_path)) {
        free(result);
        return NULL;
    }

    snprintf(search_path, sizeof(search_path), "%s\\*", dir);

    WIN32_FIND_DATA find_file_data;
    HANDLE hFind = FindFirstFile(search_path, &find_file_data);

    if (hFind == INVALID_HANDLE_VALUE) {
        free(result);
        return NULL;
    }

    do {
        if (!(find_file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            const char* filename = find_file_data.cFileName;

            int match = 0;

            if (strcmp(pattern, "*") == 0) {
                match = 1;
            }
            else if (strncmp(pattern, "*.", 2) == 0) {
                const char* ext = pattern + 2;
                const char* file_ext = strrchr(filename, '.');

                if (file_ext != NULL) {
                    if (_stricmp(file_ext + 1, ext) == 0) {
                        match = 1;
                    }
                }
            }
            else {
                if (strstr(filename, pattern) != NULL) {
                    match = 1;
                }
            }

            if (match) {
                if (!first_file) {
                    strcat(result, " ");
                }
                strcat(result, ADD_STR3(dir, "/", find_file_data.cFileName));
                first_file = 0;
            }
        }
    } while (FindNextFile(hFind, &find_file_data) != 0);

    FindClose(hFind);

    return result;
}

int appendToFile(const char* filename, const char* text) {
    FILE* file;
    long file_size;
    char* content;
    size_t text_len, total_len;

    file = fopen(filename, "rb");
    if (!file) {
        file = fopen(filename, "wb");
        if (!file) return 0;
        fwrite(text, 1, strlen(text), file);
        fclose(file);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    text_len = strlen(text);
    total_len = text_len + file_size;
    content = (char*)malloc(total_len + 1);
    if (!content) {
        fclose(file);
        return 0;
    }

    memcpy(content, text, text_len);

    if (file_size > 0) {
        fread(content + text_len, 1, file_size, file);
    }
    content[total_len] = '\0';

    fclose(file);

    file = fopen(filename, "wb");
    if (!file) {
        free(content);
        return 0;
    }

    fwrite(content, 1, total_len, file);
    fclose(file);
    free(content);

    return 1;
}

int exist_directory(const char* path)
{
    DWORD attrib = GetFileAttributesA(path);
    if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return 0; 
    }

    char fixed_path[MAX_PATH];
    strcpy(fixed_path, path);
    for (int i = 0; fixed_path[i]; i++) {
        if (fixed_path[i] == '/') fixed_path[i] = '\\';
    }

    attrib = GetFileAttributesA(fixed_path);
    if (attrib != INVALID_FILE_ATTRIBUTES && (attrib & FILE_ATTRIBUTE_DIRECTORY)) {
        return 0; 
    }

    return GetLastError();
}

int write_file(const char* path, const char* format, ...)
{
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }

    va_list args;

    va_start(args, format);
        vfprintf(file, format, args);
    va_end(args);

    fclose(file);
    return 0;
}

int write_file_va(const char* path, const char* format, va_list args)
{
    FILE* file = fopen(path, "w");
    if (file == NULL) {
        return -1;
    }
    va_list args_copy;
    va_copy(args_copy, args);
      int result = vfprintf(file, format, args_copy);
    va_end(args_copy);

    fclose(file);
    return result;
}

int file_copy(const char* from, const char* to)
{
    if (!from || !to) return -1;

    FILE* src = fopen(from, "rb");
    if (!src) return -2;

    FILE* dst = fopen(to, "wb");
    if (!dst) {
        fclose(src);
        return -3;
    }

    int ch;
    while ((ch = fgetc(src)) != EOF) {
        if (fputc(ch, dst) == EOF) {
            fclose(src);
            fclose(dst);
            return -4; 
        }
    }

    fclose(src);
    fclose(dst);
    return 0;
}


const char* get_file_name(const char* path) {
    if (!path || *path == '\0') {
        return "";
    }
    const char* last_slash = strrchr(path, '/');
    const char* last_backslash = strrchr(path, '\\');

    const char* last_separator = (last_slash > last_backslash) ? last_slash : last_backslash;

    if (last_separator) {
        return last_separator + 1; 
    }

    return path;
}

const char* read_file(const char* path) {
    if (!path) return NULL;

    FILE* file = fopen(path, "rb");
    if (!file) return NULL;

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (file_size <= 0) {
        fclose(file);
        return NULL;
    }

    char* content = malloc(file_size + 1);
    if (!content) {
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(content, 1, file_size, file);
    fclose(file);

    if (bytes_read != file_size) {
        free(content);
        return NULL;
    }

    content[file_size] = '\0'; 
    return content;
}