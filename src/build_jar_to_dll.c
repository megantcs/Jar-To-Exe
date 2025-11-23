#include "header/convertor.h"
#include "header/hex_tools.h"
#include "header/files.h"

void build_jar_to_dll(struct buildItem* const _this)
{
	ERROR_OF_NULL(global.launch_class, "$ [Config] [requires] param [class] is empty\n");
	ERROR_OF_NULL(global.launch_method, "$ [Config] [requires] param [method] is empty\n");

	char* hex_path = str_spr_str("%s%s",global.template_jar_to_dll, "/header.h");
	int hex_state = file_to_hex_dump(_this->input, hex_path);

	log(DebugLogger, "hex set path: %s", hex_path);
	if (hex_state != 0) {
		log(MainLogger, "error hex $template_jar_to_dll\\header.h, error code %d", hex_state);
		return;
	}

	char* content = str_spr_str("%s\n%s\n%s\n%s\n%s\n%s",
		ADD_STR2("#define __CLASS__ ", SPR_STR("\"%s\"", global.launch_class)),
		ADD_STR2("#define __METHOD__ ", SPR_STR("\"%s\"", global.launch_method)),
		ADD_STR2("#define __CLASS_LOADER ", "\"un\""), // xd
		"", 
		"", 
		""  
	);

	appendToFile(hex_path, content);


	char* files_cpp = get_str_files_in_dir(global.template_jar_to_dll, "*.cpp");
	char* files_h = get_str_files_in_dir(global.template_jar_to_dll, "*.h");
	char* command = str_spr_str("%s %s", files_cpp, files_h);

	compile_gnu(command);
end:
	log(MainLogger, "finished build dll, code: %d", global.exit_code);
	return;
}

