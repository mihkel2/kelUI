#pragma once
#include <string>
#include <vector>
#include <vulkan/vulkan.h>

namespace kel {
    struct KelPose {
        float x, y, z;
        float rx, ry, rz, rw;
    };

    struct Tab {
        std::string name;
        KelPose offset;
    };

    struct KelInstance {
        int width, height;
        bool enabled;
        std::vector<Tab> tabs;
        int active_tab = 0;

        KelInstance() : width(0), height(0), enabled(true) {}

        void setsize(int w, int h) {
            width = w;
            height = h;
        }

        bool isenabled() const {
            return enabled;
        }

        static void start();

        void createtab(const std::string& name, float offX, float offY, float offZ);

        bool button(const std::string& text);
        void checkbox(const std::string& text, bool* val);
        void sliderint(const std::string& text, int* val, int min, int max);
        void sliderfloat(const std::string& text, float* val, float min, float max);

        void rect(float x, float y, float w, float h, float r, float g, float b, float a);
        void text(float x, float y, const std::string& str, float r, float g, float b, float a);

        void updatecontrollerpose(KelPose pose);
        void rendervulkan(VkCommandBuffer cmd, VkExtent2D extent, VkImage img, VkImageView view);
    };

    KelInstance* getapi();
}
