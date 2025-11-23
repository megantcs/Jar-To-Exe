#ifndef _VAR210_USE_
#define _VAR210_USE_

#include <string>
#include <vector>
#include <memory>

#include "jni.h"
#include "minz.h"
#include "var310.h"
#include <iostream>

class var210 {
public:
	var210(jobject inst) : instance(inst) {}
	var210(const std::string& class_name, const std::string& super_name, const std::vector<std::string>& interfaces_names, unsigned char* bytes, size_t size);

	~var210();
	bool define(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader);
	jclass findClass(const std::string& class_path, jobject class_loader) const;
	bool is_defined = false;
private:
	jobject instance;

	bool define_super		(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader);
	bool define_interfaces	(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader);

	std::string class_name;
	std::string super_name;
	std::vector<std::string> interfaces_names;
	unsigned char* bytes;
	size_t size;
};

inline var210::var210
(
	const std::string& class_name,
	const std::string& super_name,
	const std::vector<std::string>& interfaces_names,
	unsigned char* bytes, size_t size) :
	class_name(class_name),
	super_name(super_name),
	interfaces_names(interfaces_names),
	bytes(bytes),
	size(size)
{

}

inline var210::~var210()
{
	if (bytes)
		mz_free(bytes);
}

inline jclass defineClass(const jbyte* classBytes, jsize size, jobject class_loader)
{
	std::cout << class_loader << " CLASS LOADER - TARGET\n";
	std::cout << size << "\tSIZE\n";

	jclass jaclass = var310::getVar1()->DefineClass(nullptr, class_loader, classBytes, size);

	if (!jaclass)
	{
		return nullptr;
	}
	return jaclass;
}

inline bool var210::define(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader)
{
	if (is_defined)
		return true;

	if (!define_super(to_define, classLoader)) {
		std::cout << "R2\n";
		return false;
	}
	if (!define_interfaces(to_define, classLoader)) 
	{
		std::cout << "R1\n";
		return false;
	}

	jclass defined = defineClass(reinterpret_cast<const jbyte *>(bytes), size, classLoader);
	if (!defined) {
		std::cout << "R3\n";
		return false;
	}

	is_defined = true;
	return true;

	return false;
}

inline jclass var210::findClass(const std::string &class_path, jobject class_loader) const
{
#define env var310::getVar1()

	static jmethodID findClass_mid = nullptr;
	if (!findClass_mid)
	{
		jclass classLoaderClass = env->FindClass("java/lang/ClassLoader");
		findClass_mid = env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
		env->DeleteLocalRef(classLoaderClass);
	}
	jstring str = env->NewStringUTF(class_path.c_str());
	jclass found_class = (jclass)env->CallObjectMethod(class_loader, findClass_mid, str);
	env->DeleteLocalRef(str);
	return found_class;
#undef env
}

inline bool var210::define_super(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader)
{
	if (super_name.empty())
		return true;

	for (std::unique_ptr<var210>& cb : to_define)
	{
		if (!cb->is_defined && cb->class_name == super_name)
		{
			return cb->define(to_define, classLoader);
		}
	}

	return true;
}

inline bool var210::define_interfaces(std::vector<std::unique_ptr<var210>>& to_define, jobject& classLoader)
{
	if (interfaces_names.empty())
		return true;

	for (const std::string& interface_name : interfaces_names)
	{
		for (std::unique_ptr<var210>& cb : to_define)
		{
			if (!cb->is_defined && cb->class_name == interface_name)
			{
				if (!cb->define(to_define, classLoader)) return false;
			}
		}
	}

	return true;
}

#endif