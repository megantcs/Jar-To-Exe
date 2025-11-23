#ifndef BUILD_SYS
#define BUILD_SYS

/* for window, 260 */
#define MAX_PATH 255

/* supported compilers */
#define GCC "g++"
#define CLANG "clang++"

/* build sys for compiler: clang++ g++ */
typedef struct BuildSys {

	char compiler[MAX_PATH];
	char output[MAX_PATH];
	
	char* custom_args;
	char* files;
}BuildSys;

#define BUILD_SYS_METHOD(name) build_sys_##name

/* non-static */
BuildSys*			BUILD_SYS_METHOD(ctor)();
const char*			BUILD_SYS_METHOD(get_cmd)(BuildSys* _this);
void				BUILD_SYS_METHOD(dtor)(BuildSys* _this);
void				BUILD_SYS_METHOD(add_file)(BuildSys* _this, const char* file);
void				BUILD_SYS_METHOD(add_arg)(BuildSys* _this, const char* arg);
void				BUILD_SYS_METHOD(set_output)(BuildSys* _this, const char* output);
void				BUILD_SYS_METHOD(set_compiler)(BuildSys* _this, const char* compiler);
void				BUILD_SYS_METHOD(clear_files)(BuildSys* _this);
void			    BUILD_SYS_METHOD(clear_args)(BuildSys* _this);
void				BUILD_SYS_METHOD(build)(BuildSys* _this);


/* static */
const char*			BUILD_SYS_METHOD(first_detect_compiler)();

#endif
