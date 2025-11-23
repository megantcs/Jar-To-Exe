#include "jvmti.h"
#include "jni.h"

#ifndef _VAR_310_H_
#define _VAR_310_H_

	class var310 {
	public:
		static void setVar(JNIEnv* env, JavaVM* vm, jvmtiEnv* jvmti);

		static JNIEnv*   getVar1();
		static JavaVM*   getVar2();
		static jvmtiEnv* getVar3();

		inline static JNIEnv* _env;
		inline static JavaVM* _vm;
		inline static jvmtiEnv* _jvmti;
	};
#endif