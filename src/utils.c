#include "header/utils.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <windows.h>

#define TRUE 1
#define FALSE 0
#define nullptr 0

#define SYM_FOR(argc, argv) for(int i = 0; i < argc; ++i) 

int has_argument(int argc, char** argv, const char* searched)
{
	for (int i = 0; i < argc; i++) 
	{
		if (strcmp(argv[i], searched) == 0) return TRUE;
	}
	return FALSE;
}

int find_argument(int argc, char** argv, const char* searched)
{
	int pos = InvalidPos;
	for (int i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], searched) == 0) 
		{
			pos = i;
			break;
		}
	}
	return pos;
}



const char* get_argument_add_pos(int argc, char** argv, const char* searched)
{
	int i = find_argument(argc, argv, searched);
	if (i == InvalidPos) return InvalidPos;

	int tPos = i + 1;
	if (tPos > argc) return InvalidPos;

	return argv[tPos];
}

int is_empty(const char* t)
{
	if (strlen(t) == 0) return 1;
	if(t[0] == '\0') return 1;

	return 0;
}

char* str_spr_str(const char* format, ...)
{
	if (format == NULL) {
		return strdup(""); 
	}

	char stack_buf[1024];
	va_list list;

	va_start(list, format);
	int result = vsnprintf(stack_buf, sizeof(stack_buf), format, list);
	va_end(list);

	if (result < 0) {
		return strdup("");
	}

	if (result < (int)sizeof(stack_buf)) {
		return strdup(stack_buf);
	}

	char* heap_buf = (char*)malloc(result + 1);
	if (heap_buf == NULL) {
		return strdup(stack_buf); 
	}

	va_start(list, format);
	vsnprintf(heap_buf, result + 1, format, list);
	va_end(list);

	return heap_buf;
}

char* add_str2_macro(const char* first, const char* second)
{
	return str_spr_str("%s%s", first, second);
}
char* add_str3_macro(const char* first, const char* second, const char* tr)
{
	return str_spr_str("%s%s%s", first, second, tr);
}

void substr(char* str, char* sub, int start, int len) {
	memcpy(sub, &str[start], len);
	sub[len] = '\0';
}

const char* get_argument_splits(PROGRAM_ARGUMENTS, const char* searched, char del)
{
	SYM_FOR(argc, argv)
	{
		const char* line = argv[i];

		if (strncmp(line, searched, strlen(searched)) != 0) continue;

		int pos = str_get_pos(line, del);
		if (pos == InvalidPos) continue;

		if (pos < (int)strlen(searched)) continue;

		char* second = (char*)malloc(strlen(line) - pos);
		if (second == NULL) return NULL;

		substr(line, second, pos + 1, strlen(line) - pos - 1);

		return second;
	}

	return NULL;
}

int find_str(const char* str, const char* substring)
{
	char* pos = strstr(str, substring);
	return pos ? (int)(pos - str) : -1;
}

int str_get_pos(const char* source, char del)
{
	size_t size = strlen(source);
	size_t pos = InvalidPos;

	for (int i = 0; i < size; i++) {
		if (source[i] == del) {
			pos = i;
			break;
		}
	}

	return pos;
}

int get_type_argumts2s2t(PROGRAM_ARGUMENTS, const char* first, int first_type, const char* second, int second_type)
{
	if (has_argument(SET_PROGRAM_ARGUMENTS, first)) return first_type;
	if (has_argument(SET_PROGRAM_ARGUMENTS, second)) return second_type;

	return InvalidPos;
}

void critical_assert_dialog(const char* file, int line, const char* expression, const char* function)
{
	char message[1024];
	char title[256];

	snprintf(message, sizeof(message),
		"    CRITICAL ASSERTION FAILED \n\n"
		"File: %s\n"
		"Line: %d\n"
		"Function: %s\n"
		"Expression: %s\n\n"
		"What do you want to do?",
		file, line, function, expression);

	snprintf(title, sizeof(title), "Critical Error - %s", function);

	int result = MessageBoxA(NULL, message, title,
		MB_ICONERROR | MB_ABORTRETRYIGNORE | MB_DEFBUTTON1);

	switch (result) {
	case IDABORT:
		ExitProcess(1);  
		break;
	case IDRETRY:
		DebugBreak();    
		break;
	case IDIGNORE:
		return;          
		break;
	}
}
