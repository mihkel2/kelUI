#pragma once
#include <string>
#include <vulkan/vulkan.h>

namespace kel {
    struct KelInstance {
        int width, height;
        bool enabled;

        KelInstance() : width(0), height(0), enabled(true) {}

        void setsize(int w, int h) {
            width = w;
            height = h;
        }

        bool isenabled() const {
            return enabled;
        }

        static void start();

        void button(const std::string& name, bool* state, void(*callback)());


        void rendervulkan(VkCommandBuffer cmd, VkExtent2D extent, VkImage img, VkImageView view);
    };

    KelInstance* getapi();
}
