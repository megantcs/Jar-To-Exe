#include "header/convertor.h"
#include <stdio.h>

int main(int argc, char** argv) 
{
	const char* p = get_argument_splits(SET_PROGRAM_ARGUMENTS, "config", '=');
	printf("=> %s\n", SAFE_GET(p));

	global = parse_global_settings("config.txt");

}
