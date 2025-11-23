#pragma once
#include <memory>
#include "var210.h"
#include "var310.h"
#include "minz.h"

namespace Java {
	class JarLoader
	{
	public:
		JarLoader(jobject classLoader, jobject parent);
		~JarLoader();
	bool Java::JarLoader::register_resources(
    const std::vector<std::pair<std::string, std::vector<uint8_t>>>& resources
);

		bool load_jar(const uint8_t jar_bytes[], size_t bytes_number);
		bool load_jar(const std::string& jar_path);

	private:
		struct OriginalThreadData
		{
			jobject thread;
			jobject originalClassLoader;
		};

		JNIEnv* jni;
		jobject classLoader;
		jobject originalSystemClassLoader;
		std::vector<OriginalThreadData> originalThreadData{};

		bool load_jar_from_archive(mz_zip_archive& archive);
	};
}