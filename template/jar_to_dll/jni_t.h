#include "jni.h"
#include <string_view>

#include "jvmti.h"

struct JNI_T
{
    static JNIEnv* env;
    static jvmtiEnv* jvmti;

    static jclass findClassUseBootClassLoader(std::string_view path);
};

inline JNIEnv* JNI_T::env;
inline jvmtiEnv* JNI_T::jvmti;

inline jclass JNI_T::findClassUseBootClassLoader(std::string_view path) {
    jclass* loaded_classes = nullptr;
    jint loaded_classes_count = 0;
    jclass found_class = nullptr;
    jvmti->GetLoadedClasses(&loaded_classes_count, &loaded_classes);
    for (jint i = 0; i < loaded_classes_count; ++i)
    {
        char* signature_buffer = nullptr;
        jvmti->GetClassSignature(loaded_classes[i], &signature_buffer, nullptr);
        std::string signature = signature_buffer;
        jvmti->Deallocate((unsigned char*)signature_buffer);
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
    jvmti->Deallocate((unsigned char*)loaded_classes);
    return found_class;
}

