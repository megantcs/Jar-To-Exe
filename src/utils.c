#include "header/utils.h"
#include <string.h>

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


void substr(char* str, char* sub, int start, int len) {
	memcpy(sub, &str[start], len);
	sub[len] = '\0';
}

const char* get_argument_add_pos(int argc, char** argv, const char* searched)
{
	int i = find_argument(argc, argv, searched);
	if (i == InvalidPos) return InvalidPos;

	int tPos = i + 1;
	if (tPos > argc) return InvalidPos;

	return argv[tPos];
}

const char* get_argument_splits(PROGRAM_ARGUMENTS, const char* searched, char del)
{
	SYM_FOR(argc, argv)
	{
		const char* line = argv[i];
		int pos = str_get_pos(line, del);
		if (pos == InvalidPos) continue;

		const char* second[100];
		substr(line, second, pos + 1, strlen(line));


		return second;
	}

	return InvalidPos;
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
