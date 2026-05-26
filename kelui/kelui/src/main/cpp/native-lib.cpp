#include <jni.h>
#include <string>
#include "kel.hpp"

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_kelui_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {

    kel::KelInstance::start();
    auto api = kel::getapi();

    // Example Widget Usage
    static bool my_checkbox = true;
    static int my_slider = 50;

    api->button("KelUI button testing");
    api->checkbox("Enable Mod", &my_checkbox);
    api->sliderint("Speed Multiplier", &my_slider, 0, 100);


    return env->NewStringUTF(hello.c_str());
}
