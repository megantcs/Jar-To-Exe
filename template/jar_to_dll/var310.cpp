#include "var310.h"



void var310::setVar(JNIEnv* env, JavaVM* vm, jvmtiEnv* jvmti)
{
	_env = env;
	_vm = vm;
	_jvmti = jvmti;
}

JNIEnv* var310::getVar1()
{
	return _env;
}

JavaVM* var310::getVar2()
{
	return _vm;
}

jvmtiEnv* var310::getVar3()
{
	return _jvmti;
}
