#include <jni.h>
#include <android/log.h>
#include "../api/kel.hpp"

// ill make a YT video detailing more on the menu
// last thing code may look less user friendly and im coding super lazy
// its nearly 1:00 XD


void updatevrmenuposition(float posX, float posY, float posZ,
                             float rotX, float rotY, float rotZ, float rotW) {
    kel::KelPose pose;
    pose.x = posX; pose.y = posY; pose.z = posZ;
    pose.rx = rotX; pose.ry = rotY; pose.rz = rotZ; pose.rw = rotW;

    kel::getapi()->updatecontrollerpose(pose);
}

void initexternalstuff() {


    auto api = kel::getapi();



    // tab creation docs api->create_tab("Combat",  0.1f, 0.0f, -0.3f);

}
