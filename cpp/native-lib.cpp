#include <jni.h>
#include <string>
#include "kel.hpp"

extern "C" void init_kelui();

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kelui_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    init_kelui();
    auto api = kel::getapi();

    std::string status = "";
    return env->NewStringUTF(status.c_str());
}
