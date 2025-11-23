#pragma once
#include <string_view>
#include <array>
#include "MappingsCore.h"
#include "jni.h"
#include "jvmti.h"
#include "var210.h"
#include "jni_t.h"

#undef env

struct JNI {
	JNIEnv* env;
	jvmtiEnv* jvmt;
};

class Transformer
{
public:
	Transformer(JNI& jni, jobject& minecraftClassLoader);
	~Transformer();
	bool retransform();
private:
	JNI& jni;
	jobject& minecraftClassLoader;
	inline static bool disable = false;

	struct TransformData
	{
		std::string_view target_class;
		std::string_view patcher_name;
	};
	inline static constexpr TransformData to_transform[] =
	{
		{Mappings::minecraft_class, "patch_net_minecraft_client_MinecraftClient"},
	};

	static void JNICALL ClassFileLoadHook_callback(jvmtiEnv* jvmti_env,
		JNIEnv* jni_env,
		jclass class_being_redefined,
		jobject loader,
		const char* name,
		jobject protection_domain,
		jint class_data_len,
		const unsigned char* class_data,
		jint* new_class_data_len,
		unsigned char** new_class_data);
};
