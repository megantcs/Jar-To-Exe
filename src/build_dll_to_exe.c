#include "header/convertor.h"
#include "header/hex_tools.h"
#include "header/files.h"

void build_dll_to_exe(struct buildItem* const _this)
{
	char* hex_path = str_spr_str("%s%s", global.template_dll_to_exe, "/header.h");
	int hex_state = file_to_hex_dump(_this->input, hex_path);

	log(DebugLogger, "hex set path: %s", hex_path);
	if (hex_state != 0) {
		log(MainLogger, "error hex $template_dll_to_exe\\header.h, error code %d", hex_state);
		return;
	}

	char* files_cpp = get_str_files_in_dir(global.template_dll_to_exe, "*.cpp");
	char* files_h = get_str_files_in_dir(global.template_dll_to_exe, "*.h");
	char* command = str_spr_str("%s %s", files_cpp, files_h);

	compile_gnu(command);
end:
	log(MainLogger, "finished build exe, code: %d", global.exit_code);
	return;
}