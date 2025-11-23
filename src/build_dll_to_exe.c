#include "header/convertor.h"
#include "header/hex_tools.h"
#include "header/files.h"
#include "header/build_sys.h"

void build_dll_to_exe(struct buildItem* const _this)
{
	char* hex_path = str_spr_str("%s%s", global.template_dll_to_exe, "/header.h");
	int hex_state = file_to_hex_dump(_this->input, hex_path);

	log(DebugLogger, "hex set path: %s", hex_path);
	if (hex_state != 0) {
		log(MainLogger, "error hex $template_dll_to_exe\\header.h, error code %d", hex_state);
		global.exit_code = -1;
		goto end;
	}

	char* files_cpp = get_str_files_in_dir(global.template_dll_to_exe, "*.cpp");

	BuildSys* builder = build_sys_ctor();

	build_sys_add_arg(builder, files_cpp);
	build_sys_set_output(builder, global.output);
	build_sys_set_compiler(builder, global.gnu_compiler);
	build_sys_build(builder);

	build_sys_dtor(builder);
end:
	log(MainLogger, "finished build exe, code: %d", global.exit_code);
	return;
}