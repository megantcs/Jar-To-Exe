#ifndef UTILS_H
#define UTILS_H

#include "header/terminal_attribute.h"

#define InvalidPos -1
#define PROGRAM_ARGUMENTS int argc, char** argv
#define SET_PROGRAM_ARGUMENTS argc, argv
#define STR_GET(str1, str2) strlen(str1) == 0? str2 : str1;
#define SAFE_GET(str) str != InvalidPos? str : ""

#define SET_OR_NULL(str, new_value) if(!str) str = new_value
#define COPY_OR_NULL(dest, src) \
    do { \
        if ((dest) != NULL && (src) != NULL) { \
            strncpy((dest), (src), sizeof(dest) - 1); \
            (dest)[sizeof(dest) - 1] = '\0'; \
        } \
    } while(0)

#define WARN_OF_NULL(str, format) if(!str || is_empty(str)){ WARNING(format); }   
#define ERROR_OF_NULL(str, format) if(!str || is_empty(str)){ ERROR(format); goto end; }   
#define ASSERT(expression, format) if(!expression) WARNING(format)

#define ADD_STR2(first, second) add_str2_macro(first, second)
#define ADD_STR3(first, second, tr) add_str3_macro(first, second, tr)
#define SPR_STR(format)  str_spr_str(format)
#define COPY_STR(dest, src)\
strncpy((dest), (src), sizeof(dest) - 1); \
(dest)[sizeof(dest) - 1] = '\0'; \

#define COLOR_A(C, ...) \
printf(C);\
printf(__VA_ARGS__);\
printf(TERMINAL_RESET);

#define ERROR(...) \
COLOR_A(TERMINAL_BG_RED, __VA_ARGS__)


#define SUCCESS(...)\
COLOR_A(TERMINAL_BG_BRIGHT_GREEN TERMINAL_FG_WHITE, __VA_ARGS__)

#define WARNING(...)\
COLOR_A(TERMINAL_FG_RED, __VA_ARGS__)

#define RUNTIME_CHECK_1(expression) if(!expression) {\
ERROR("[Runtime-Error-Check-#1] [%s:%d]: %s\n", __FILE__, __LINE__, #expression);\
goto end;\
}

#define RUNTIME_CHECK_2(expression) if(!expression) {\
ERROR("[Runtime-Error-Check-#2] [%s:%d]: %s\n", __FILE__, __LINE__, #expression);\
exit(2);\
}

#define RUNTIME_CHECK_3(expression) if(!expression) {\
ERROR("[Runtime-Error-Check-#3] [%s:%d]: %s\n", __FILE__, __LINE__, #expression);\
abort();\
}

#define DEBUG_ASSERT(expression) \
    do { \
        if (!(expression)) { \
            critical_assert_dialog(__FILE__, __LINE__, #expression, __func__); \
        } \
    } while(0)



#define MS_TO_SECOND(d) d / 1000

int is_empty(const char* t);
char* str_spr_str(const char* format, ...);
char* add_str2_macro(const char* first, const char* second);
char* add_str3_macro(const char* first, const char* second, const char* tr);

int has_argument(PROGRAM_ARGUMENTS, const char* searched);
int find_argument(PROGRAM_ARGUMENTS, const char* searched);

const char* get_argument_add_pos(PROGRAM_ARGUMENTS, const char* searched);
const char* get_argument_splits(PROGRAM_ARGUMENTS, const char* searched, char del);
int find_str(const char* str, const char* substring);
int str_get_pos(const char* source, char del);

int get_type_argumts2s2t(PROGRAM_ARGUMENTS,
	const char* first, int first_type,
	const char* second, int second_type);

void critical_assert_dialog(const char* file, int line, const char* expression, const char* function);

typedef const char* string;

#endif