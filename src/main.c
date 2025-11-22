#include "header/convertor.h"
#include "header/hex_tools.h"

#include <stdio.h>

int main(int argc, char** argv) 
{
	const char* path = "config.txt";
	const char* custom_path = get_argument_splits(SET_PROGRAM_ARGUMENTS, "config", '=');
	if (custom_path != InvalidPos) 
	{
		path = custom_path;
	}

	global = parse_global_settings("config.txt");

	file_to_hex_dump("config.txt", "config.h");
}
