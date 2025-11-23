/*
source.cpp

  Auto generated project for compiling "DLL"
  YOU CAN'T TOUCH IT

*/

#include "header.h";
#include <iostream>
#include "jni.h"
#include "jvmti.h"
#include "var310.h"
#include "JarLoader.h"
#include <Windows.h>
#include "Transformer.hpp"
#undef env

JNIEnv* env;
JavaVM* vm;
jvmtiEnv* JVMTIEnv;

using namespace Java;

/*
	INSTANCE FIND CLASS FROM CUSTOM CLASS LOADER
*/
jclass findClassFromClassLoader(std::string class_path, jobject classloader)
{
	auto jni = env;
	for (char& c : class_path)
		if (c == '/') c = '.';
	static jmethodID findClass_mid = nullptr;
	if (!findClass_mid)
	{
		jclass classLoaderClass = jni->FindClass("java/lang/ClassLoader");
		findClass_mid = jni->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		jni->DeleteLocalRef(classLoaderClass);
	}
	jstring str = jni->NewStringUTF(class_path.c_str());
	jclass found_class = (jclass)jni->CallObjectMethod(classloader, findClass_mid, str);
	jni->DeleteLocalRef(str);
	return found_class;
}

/*
    INSTANCE MINECRAFT THREAD CLASS LOADER
*/

jobject getThreadContextClassLoaderClassLoader(std::string cls)
{

	jobject classLoader = nullptr;
    jclass c_Thread = env->FindClass("java/lang/Thread");

    jclass c_Map = env->FindClass("java/util/Map");

    jclass c_Set = env->FindClass("java/util/Set");

    jclass c_ClassLoader = env->FindClass("java/lang/ClassLoader");

    jmethodID mid_getAllStackTraces = env->GetStaticMethodID(c_Thread, "getAllStackTraces", "()Ljava/util/Map;");

    jmethodID mid_keySet = env->GetMethodID(c_Map, "keySet", "()Ljava/util/Set;");

    jmethodID mid_toArray = env->GetMethodID(c_Set, "toArray", "()[Ljava/lang/Object;");

    jmethodID mid_getContextClassLoader = env->GetMethodID(c_Thread, "getContextClassLoader", "()Ljava/lang/ClassLoader;");

    jmethodID mid_getName = env->GetMethodID(c_Thread, "getName", "()Ljava/lang/String;");

    jobject obj_stackTracesMap = env->CallStaticObjectMethod(c_Thread, mid_getAllStackTraces);

    jobject obj_threadsSet = env->CallObjectMethod(obj_stackTracesMap, mid_keySet);

    jobjectArray threads = (jobjectArray)env->CallObjectMethod(obj_threadsSet, mid_toArray);

    jint szThreads = env->GetArrayLength(threads);

    for (int i = 0; i < szThreads; i++)
    {
        jobject thread = env->GetObjectArrayElement(threads, i);
        jstring name = (jstring)env->CallObjectMethod(thread, mid_getName);
        const char* nameChars = env->GetStringUTFChars(name, NULL);

        if (std::string(nameChars) == cls)
        {
         
            env->ReleaseStringUTFChars(name, nameChars);
            jobject classLoaderObj = env->CallObjectMethod(thread, mid_getContextClassLoader);
            classLoader = classLoaderObj;
        }
        // not free
    }
    return classLoader;
}


/*
	INSTANCE FIND CLASS FROM BOOT CLASS LOADER
*/
jclass findClassUseBootClassLoader(std::string_view path)
{
	jclass* loaded_classes = nullptr;
	jint loaded_classes_count = 0;
	jclass found_class = nullptr;
	JVMTIEnv->GetLoadedClasses(&loaded_classes_count, &loaded_classes);
	for (jint i = 0; i < loaded_classes_count; ++i)
	{
		char* signature_buffer = nullptr;
		JVMTIEnv->GetClassSignature(loaded_classes[i], &signature_buffer, nullptr);
		std::string signature = signature_buffer;
		JVMTIEnv->Deallocate((unsigned char*)signature_buffer);
		signature = signature.substr(1, signature.size() - 2);
		if (signature == path)
		{
			found_class = (jclass)env->NewLocalRef(loaded_classes[i]);
			break;
		}
	}
	for (jint i = 0; i < loaded_classes_count; ++i)
	{
		env->DeleteLocalRef(loaded_classes[i]);
	}
	JVMTIEnv->Deallocate((unsigned char*)loaded_classes);
	return found_class;
}

/*
	INSTANCE JAVA URLCLASSLOADER
*/
jobject create(const std::string& search_url, jobject parent)
{
	jclass urlClass = env->FindClass("java/net/URL");
	jmethodID urlContructor = env->GetMethodID(urlClass, "<init>", "(Ljava/lang/String;)V");
	jstring str = env->NewStringUTF(search_url.c_str());
	jobject url = env->NewObject(urlClass, urlContructor, str);
	jobjectArray urls = env->NewObjectArray(1, urlClass, url);
	jclass URLClassLoaderClass = env->FindClass("java/net/URLClassLoader");
	jmethodID URLClassLoaderContructor = env->GetMethodID(URLClassLoaderClass, "<init>", "([Ljava/net/URL;Ljava/lang/ClassLoader;)V");
	jobject URLClassLoader_o = env->NewObject(URLClassLoaderClass, URLClassLoaderContructor, urls, parent);

	env->DeleteLocalRef(urlClass);
	env->DeleteLocalRef(url);
	env->DeleteLocalRef(str);
	env->DeleteLocalRef(urls);
	env->DeleteLocalRef(URLClassLoaderClass);

	return URLClassLoader_o;
}

/*
   INSTANCE CLASS LOADER FROM CLASS
*/
jobject getClassloader(const std::string& class_name)
{
	jclass found = findClassUseBootClassLoader(class_name);
	if (!found)
		return nullptr;

	jobject classLoader = nullptr;
	JVMTIEnv->GetClassLoader(found, &classLoader);
	return classLoader;
}

void load(HMODULE module_)
{

 FILE* buffer = nullptr;
    AllocConsole();
    freopen_s(&buffer, "CONOUT$", "w", stdout);

	JNI_GetCreatedJavaVMs(&vm, 1l, nullptr);

	if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_9 == JNI_OK))
	{
		vm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
	}
	vm->GetEnv((void**)&JVMTIEnv, JVMTI_VERSION_1_2);

	var310::setVar(env, vm, JVMTIEnv);
	
	jobject minecraft_class_loader;
	if(__CLASS_LOADER == "un") 
	{
		minecraft_class_loader = getThreadContextClassLoaderClassLoader("Render thread");
	}
	else {
	minecraft_class_loader		 = getClassloader(__CLASS_LOADER);
	}
	jobject client_class_loader			 = create("file:///C:/Windows/win.ini", minecraft_class_loader);

	std::cout << minecraft_class_loader << "\n";
	std::cout << client_class_loader << "\n";

	JarLoader jar
	(
		minecraft_class_loader,
		client_class_loader
	);

	if (jar.load_jar(rawData, __SIZE__)) 
	{

	}

	JNI jn2{env, var310().getVar3()};


	bool isCalling = false;
	auto _ = findClassUseBootClassLoader(__CLASS__);
	if (_) 
	{
		jmethodID id = env->GetStaticMethodID(_, __METHOD__, "()V");
		if(!id) 
		{
			auto scReplyClass = _;
			auto scInfo_class = env->NewGlobalRef(scReplyClass);
			auto scInfo_constructor = env->GetMethodID((jclass)scInfo_class, "<init>", "()V");
			auto scObject = env->NewObject((jclass)scInfo_class, scInfo_constructor);

			auto scMethod1 = env->GetMethodID((jclass)scInfo_class, __METHOD__, "()V");
			if (scMethod1) {
				env->CallVoidMethod(scObject, scMethod1);
			
				isCalling = true;
			}
		}
		else 
		{
			isCalling = true;
			env->CallStaticVoidMethod(_, id);
		}
	}
	else { }

	if(isCalling) {
		std::cout << "Success calling!\n";
	}
	else std::cout << "error calling!\n";

	while(1)
	{

	}

	FreeLibraryAndExitThread(module_, 0ul);
}

DWORD APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE handle = CreateThread(nullptr, 0ul, reinterpret_cast<PTHREAD_START_ROUTINE>(load), hModule, 0ul, nullptr);
		if (handle && handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return true;
}

