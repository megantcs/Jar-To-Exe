#ifndef UTILS_H
#define UTILS_H

#define InvalidPos -1
#define PROGRAM_ARGUMENTS int argc, char** argv
#define SET_PROGRAM_ARGUMENTS argc, argv
#define STR_GET(str1, str2) strlen(str1) == 0? str2 : str1;
#define SAFE_GET(str) str != InvalidPos? str : ""

int has_argument(PROGRAM_ARGUMENTS, const char* searched);
int find_argument(PROGRAM_ARGUMENTS, const char* searched);

const char* get_argument_add_pos(PROGRAM_ARGUMENTS, const char* searched);
const char* get_argument_splits(PROGRAM_ARGUMENTS, const char* searched, char del);

int str_get_pos(const char* source, char del);

typedef const char* string;

#endif