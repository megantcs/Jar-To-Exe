#ifndef UTILS_H
#define UTILS_H

#define InvalidPos -1
#define PROGRAM_ARGUMENTS int argc, char** argv
#define SET_PROGRAM_ARGUMENTS argc, argv

#define SAFE_GET(str) str != InvalidPos? str : ""

int has_argument(PROGRAM_ARGUMENTS, const char* searched);
int find_argument(PROGRAM_ARGUMENTS, const char* searched);

const char* get_argument_add_pos(PROGRAM_ARGUMENTS, const char* searched);
const char* get_argument_splits(PROGRAM_ARGUMENTS, const char* searched, char del);

int str_get_pos(const char* source, char del);

#endif