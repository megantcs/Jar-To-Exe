#include "JarLoader.h"
#include "ClassFileParser.h"
#include <iostream>

void setSystemClassLoader(jobject& classLoader)
{
	auto jni = var310::getVar1();
	jclass classLoaderClass = jni->FindClass("java/lang/ClassLoader");
	static jfieldID field = [&jni, classLoaderClass]()->jfieldID
		{
			if (!classLoaderClass) return nullptr;
			jfieldID fid = jni->GetStaticFieldID(classLoaderClass, "scl", "Ljava/lang/ClassLoader;");
			return fid;
		}();

	jni->SetStaticObjectField(classLoaderClass, field, classLoader);
}

Java::JarLoader::JarLoader(jobject classLoader, jobject parent) :
jni(var310::getVar1()),
classLoader(classLoader),
originalSystemClassLoader(nullptr)
{
	setSystemClassLoader(classLoader);
}

Java::JarLoader::~JarLoader()
{

}

bool Java::JarLoader::load_jar(const uint8_t jar_bytes[], size_t bytes_number)
{
	mz_zip_archive archive{};
	if (!mz_zip_reader_init_mem(&archive, jar_bytes, bytes_number, 0))
	{
		std::cout << "Error archive!\n";
		return false;
	}

	if (!load_jar_from_archive(archive))
	{
		mz_zip_reader_end(&archive);
		return false;
	}

	mz_zip_reader_end(&archive);
	return true;
}

bool Java::JarLoader::load_jar(const std::string& jar_path)
{
	mz_zip_archive archive{};
	if (!mz_zip_reader_init_file(&archive, jar_path.c_str(), 0))
	{
		std::cout << ("Incorrect jar path/format\n");
		return false;
	}

	if (!load_jar_from_archive(archive))
	{
		mz_zip_reader_end(&archive);
		std::cout << ("Could not load jar\n");
		return false;
	}

	std::cout << ("Successfully loaded jar\n");
	mz_zip_reader_end(&archive);
	return true;
}

/*
	bool Java::JarLoader::update_thread_context_classloaders(Java::ClassLoader& parent)
	{
		for (Java::Thread& thread : Java::Thread::getAllThreads())
		{
			Java::ClassLoader original_classLoader = thread.getContextClassLoader();
			if (!original_classLoader.Equals(parent))
				continue;
			originalThreadData.push_back({ thread, original_classLoader });
			thread.setContextClassLoader(classLoader);
		}
		return true;
	}


	void Java::JarLoader::restore_thread_context_classloaders()
	{
		for (OriginalThreadData& data : originalThreadData)
		{
			data.thread.setContextClassLoader(data.originalClassLoader);
		}
	}
*/

bool Java::JarLoader::register_resources(
    const std::vector<std::pair<std::string, std::vector<uint8_t>>>& resources
) {
    JNIEnv* env = this->jni;  // Access member variable via 'this'
    jobject loader = this->classLoader;  // Access member variable via 'this'

    jclass urlClassLoaderClass = env->FindClass("java/net/URLClassLoader");
    if (!urlClassLoaderClass) {
        std::cerr << "Failed to find URLClassLoader class\n";
        return false;
    }

    // Get the resource map from the class loader
    jclass classLoaderClass = env->GetObjectClass(loader);
    jfieldID resourcesField = env->GetFieldID(classLoaderClass, "resources", "Ljava/util/Map;");
    if (!resourcesField) {
        std::cerr << "Failed to find resources field in ClassLoader\n";
        return false;
    }

    jobject resourceMap = env->GetObjectField(loader, resourcesField);
    if (!resourceMap) {
        // Create new HashMap if none exists
        jclass hashMapClass = env->FindClass("java/util/HashMap");
        jmethodID hashMapInit = env->GetMethodID(hashMapClass, "<init>", "()V");
        resourceMap = env->NewObject(hashMapClass, hashMapInit);
        env->SetObjectField(loader, resourcesField, resourceMap);
    }

    jclass mapClass = env->GetObjectClass(resourceMap);
    jmethodID mapPut = env->GetMethodID(
        mapClass, "put", "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;"
    );

    for (const auto& [path, data] : resources) {
        jstring jPath = env->NewStringUTF(path.c_str());
        jbyteArray jData = env->NewByteArray(data.size());
        env->SetByteArrayRegion(jData, 0, data.size(), (jbyte*)data.data());

        env->CallObjectMethod(resourceMap, mapPut, jPath, jData);
        
        env->DeleteLocalRef(jPath);
        env->DeleteLocalRef(jData);
    }

    return true;
}

bool Java::JarLoader::load_jar_from_archive(mz_zip_archive& archive) {
    std::vector<std::unique_ptr<var210>> classes_to_define;
    std::vector<std::pair<std::string, std::vector<uint8_t>>> resources; // Путь + данные

    mz_uint file_number = mz_zip_reader_get_num_files(&archive);
    for (mz_uint i = 0; i < file_number; i++) {
        if (!mz_zip_reader_is_file_supported(&archive, i) || 
            mz_zip_reader_is_file_a_directory(&archive, i)) 
            continue;

        char filename[512] = {0};
        mz_zip_reader_get_filename(&archive, i, filename, 512);
        std::string file_path(filename);

        // Обработка .class файлов
        if (file_path.ends_with(".class")) {
            size_t size = 0;
            uint8_t* data = (uint8_t*)mz_zip_reader_extract_to_heap(&archive, i, &size, 0);
            if (!data) continue;

            ClassFileParser parser(data, size);
            auto cb = std::make_unique<var210>(
                parser.get_class_name(),
                parser.get_super_class_name(),
                parser.get_interfaces_names(),
                data, size
            );
            classes_to_define.push_back(std::move(cb));
        } 
        // Обработка ресурсов (assets/...)
        else if (file_path.starts_with("assets/")) {
            size_t size = 0;
            uint8_t* data = (uint8_t*)mz_zip_reader_extract_to_heap(&archive, i, &size, 0);
            if (data) {
                resources.emplace_back(file_path, std::vector<uint8_t>(data, data + size));
				std::cout << "[tgfds]\n";
                free(data); // Освобождаем после копирования
            }
        }
    }

    // Загрузка классов
    for (auto& classBytes : classes_to_define) {
        if (!classBytes->define(classes_to_define, classLoader)) {
            return false;
        }
    }

    // Регистрация ресурсов в ClassLoader
    if (!register_resources(resources)) {
        return false;
    }

    return true;
}


