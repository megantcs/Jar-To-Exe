#include "Transformer.hpp"
#include <thread>
#include "header.h"

bool _is_error = false;
jobject _instance_class_loader;

Transformer::Transformer(JNI& jni, jobject& minecraftClassLoader) :
	jni(jni),
    minecraftClassLoader(minecraftClassLoader)
{
    jvmtiCapabilities cap{};
    cap.can_retransform_classes = JVMTI_ENABLE;
    if (jni.jvmt->AddCapabilities(&cap) != JVMTI_ERROR_NONE)
    {
        std::cout << ("Retransform classes not supported\n");
        return;
    }

    jvmtiEventCallbacks callbacks{};
    callbacks.ClassFileLoadHook = ClassFileLoadHook_callback;
    if (jni.jvmt->SetEventCallbacks(&callbacks, sizeof(jvmtiEventCallbacks)) != JVMTI_ERROR_NONE)
    {
         std::cout << ("Failed to set event callback\n");
        return;
    }

    if (jni.jvmt->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr) != JVMTI_ERROR_NONE)
    {
         std::cout << ("Failed enable event\n");
        return;
    }

	_is_error = false;
}

Transformer::~Transformer()
{
    jni.jvmt->SetEventNotificationMode(JVMTI_DISABLE, JVMTI_EVENT_CLASS_FILE_LOAD_HOOK, nullptr);
    jvmtiEventCallbacks callbacks{};
    jni.jvmt->SetEventCallbacks(&callbacks, sizeof(jvmtiEventCallbacks));

    retransform();

    jvmtiCapabilities cap{};
    cap.can_retransform_classes = JVMTI_ENABLE;
    jni.jvmt->RelinquishCapabilities(&cap);

    //wait a bit for the classes to completely retransform
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
}

bool Transformer::retransform()
{
    std::cout << "retransform\n";
    constexpr int to_transform_count = sizeof(to_transform) / sizeof(TransformData);
    jclass to_retransform_classes[to_transform_count] = { nullptr };
    for (int i = 0; i < to_transform_count; ++i)
        to_retransform_classes[i] = JNI_T::findClassUseBootClassLoader(std::string(to_transform[i].target_class));

    jvmtiError status = jni.jvmt->RetransformClasses(to_transform_count, to_retransform_classes);

    if (status != JVMTI_ERROR_NONE)
    {
        const char* error = "jvmti unknown error";
        jni.jvmt->GetErrorName(status, (char**)&error);
        std::cout << error << '\n';

        return false;
    }

    std::cout << "success\n";

    return true;
}



void Transformer::ClassFileLoadHook_callback(jvmtiEnv* jvmti_env, JNIEnv* jni_env, jclass class_being_redefined, jobject loader, const char* name, jobject protection_domain, jint class_data_len, const unsigned char* class_data, jint* new_class_data_len, unsigned char** new_class_data)
{
    if (!class_being_redefined) {
        std::cout << "class_begin_redefined";
        return;
    }

    for (const TransformData& transform : to_transform) {
        if (transform.target_class != std::string_view(name)) continue;

        jbyteArray original_class_bytes = jni_env->NewByteArray(class_data_len);
        jni_env->SetByteArrayRegion(original_class_bytes, 0, class_data_len, (jbyte*)class_data);
        jclass Patcher = JNI_T::findClassUseBootClassLoader(__CLASS__);
        if (!Patcher)
        {
            std::cout << "patcher not found.\n";
            return;
        }
        jmethodID patcher_method = jni_env->GetStaticMethodID(Patcher, transform.patcher_name.data(), "([B)[B");
        if (!patcher_method)
        {
            std::cout << "patcher_method not found.\n";
            return;
        }
        jbyteArray new_class_bytes = (jbyteArray)jni_env->CallStaticObjectMethod(Patcher, patcher_method, original_class_bytes);
        if (!new_class_bytes) {
            std::cout << "patcher_method return null\n";
            return;
        }

        jsize new_size = jni_env->GetArrayLength(new_class_bytes);
        if (!new_size) return;

        unsigned char* new_byte_buff = nullptr;
        jvmti_env->Allocate(new_size, &new_byte_buff);

        jni_env->GetByteArrayRegion(new_class_bytes, 0, new_size, (jbyte*)new_byte_buff);

        //return;
        std::cout << new_size << '\n';
        *new_class_data_len = new_size;
        *new_class_data = new_byte_buff;

        break;
    }
    return;
}
