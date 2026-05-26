#include <jni.h>
#include <string>
#include "kel.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kelui_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    kel::KelInstance::start();
    auto api = kel::get_api();
    api->set_size(1920, 1080);

    std::string hello = "Kel API initialized. Size: " +
                        std::to_string(api->width) + "x" +
                        std::to_string(api->height);

    return env->NewStringUTF(hello.c_str());
}
