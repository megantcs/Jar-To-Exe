#include "header/convertor.h"
#include "header/program.h"

int main(int argc, char** argv) 
{
	int exit_code = 0;
	__common__start_program(SET_PROGRAM_ARGUMENTS);

	if (argc <= 1) {
		printf("use: [dll|exe] [options]\n");

		exit_code = 4;
		goto end;
	}

	__common__global_values_check();
	__common__runtime_check_gl();

	struct buildItem item = create_build_item(global.input, global.output, get_type_argumts2s2t(SET_PROGRAM_ARGUMENTS, 
		"dll", /*=*/ JAR_TO_DLL,
		"exe", /*=*/ DLL_TO_EXE));

	build_for_item(&item);
	
	exit_code = global.exit_code;
end:
	__common__end_program();
	log(MainLogger, "finished work, in %.2f second, exit code: %d", 
		MS_TO_SECOND(__common__get_program_duration()), exit_code);

	return exit_code;
}
