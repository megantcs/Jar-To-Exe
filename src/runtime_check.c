#include "header/program.h"
#include "header/convertor.h"
#include "header/files.h"
#include "header/utils.h"
#include "header/timer.h"
#include "header/build_sys.h"

struct timer* __program_timer = nullptr;
double __program_executed_duration_ms = -1;
double __program_executed_duration_second = -1;

#define first_detect_compiler !has_arg(PARAM_NO_AUTO_DETECTED_COMPILER)? build_sys_first_detect_compiler() : "g++"

#define config_prefix "$ [Config] "

void __common__start_program(PROGRAM_ARGUMENTS)
{
	__program_timer = ctor_timer();
	start_timer(__program_timer);

	const char* path = "config.txt";
	const char* custom_path = get_argument_splits(SET_PROGRAM_ARGUMENTS, "config", '=');

	if (custom_path != InvalidPos) path = custom_path;

	global = parse_global_settings(path);
	global.debug = has_argument(SET_PROGRAM_ARGUMENTS, "--debug");
	global_settings_set_args(&global, SET_PROGRAM_ARGUMENTS);

	pack_log_folder();
}

void __common__end_program()
{
	__program_executed_duration_ms = end_timer_ms(__program_timer);
	__program_executed_duration_second = end_timer_second(__program_timer);

	dtor_timer(__program_timer);
}

void __common__runtime_check_gl()
{
	RUNTIME_CHECK_3(ExistDirectoryA(global.template_dll_to_exe) == 0 && "missing folder: template\\dll_to_exe");
	RUNTIME_CHECK_3(ExistDirectoryA(global.template_jar_to_dll) == 0 && "missing folder: template\\jar_to_dll");
}

void __common__global_values_check()
{
	WARN_OF_NULL(global.gnu_compiler, config_prefix "param [gnu_compiler] is empty | set default value: 'first detected compiler (clang, gcc)'\n");
	WARN_OF_NULL(global.input, config_prefix "param [input] is empty\n");
	WARN_OF_NULL(global.output, config_prefix "param [output] is empty\n");

	COPY_OR_NULL(global.template_dll_to_exe, "template\\dll_to_exe");
	COPY_OR_NULL(global.template_jar_to_dll, "template\\jar_to_dll");
	COPY_OR_NULL(global.gnu_compiler, first_detect_compiler);

	ASSERT(ExistDirectoryA(global.log_dir) == 0, config_prefix "param [log_dir] is empty | missing log directory.\n");
}

double __common__get_program_duration()
{
	return __program_executed_duration_ms;
}
