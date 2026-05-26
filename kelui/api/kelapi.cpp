#include "kel.hpp"
#include "../kel/ui/VulkanBackend.hpp"
#include "../kel/ui/drawing.hpp"
#include "../kel/ui/Widgets.hpp"
#include <iostream>
#include <vector>

namespace kel {
    static KelInstance* instance = nullptr;
    static VulkanBackend* vk_backend = nullptr;
    static KelPose lastcontrollerpose = {0,0,0, 0,0,0,1};

    KelInstance* getapi() {
        if (!instance) {
            instance = new KelInstance();
            vk_backend = new VulkanBackend();
        }
        return instance;
    }

    void KelInstance::start() {
        // Initialization
    }

    void KelInstance::createtab(const std::string& name, float offX, float offY, float offZ) {
        tabs.push_back({name, {offX, offY, offZ, 0, 0, 0, 1}});
    }

    bool KelInstance::button(const std::string& text) {
        return Widgets::button(text);
    }

    void KelInstance::checkbox(const std::string& text, bool* val) {
        Widgets::checkbox(text, val);
    }

    void KelInstance::sliderint(const std::string& text, int* val, int min, int max) {
        Widgets::sliderint(text, val, min, max);
    }

    void KelInstance::sliderfloat(const std::string& text, float* val, float min, float max) {
        Widgets::sliderfloat(text, val, min, max);
    }

    void KelInstance::rect(float x, float y, float w, float h, float r, float g, float b, float a) {
        Drawing::rect(x, y, w, h, {r, g, b, a});
    }

    void KelInstance::text(float x, float y, const std::string& str, float r, float g, float b, float a) {
        Drawing::text(x, y, str, {r, g, b, a});
    }

    void KelInstance::updatecontrollerpose(KelPose pose) {
        lastcontrollerpose = pose;
    }

    void KelInstance::rendervulkan(VkCommandBuffer cmd, VkExtent2D extent, VkImage img, VkImageView view) {
        if (!vk_backend || !isenabled()) return;

        Widgets::setcursor(50, 50);

        vk_backend->rendermenu(cmd, extent, img, view);
        Drawing::clear();
    }
}
