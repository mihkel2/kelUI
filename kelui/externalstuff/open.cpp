#include <jni.h>
#include <android/log.h>

// You can put your library opening/initialization logic here
void init_external_stuff() {
    __android_log_print(ANDROID_LOG_INFO, "KelAPI", "External stuff initialized");
}
