#include "kel.hpp"
#include "../kel/ui/VulkanBackend.hpp"
#include <iostream>
#include <vector>

namespace kel {
    static KelInstance* instance = nullptr;
    static VulkanBackend* vk_backend = nullptr;

    KelInstance* getapi() {
        if (!instance) {
            instance = new KelInstance();
            vk_backend = new VulkanBackend();
        }
        return instance;
    }

    void KelInstance::start() {

    }

    void KelInstance::button(const std::string& name, bool* state, void(*callback)()) {
        if (state) *state = !(*state);
        if (callback) callback();
    }

    void KelInstance::rendervulkan(VkCommandBuffer cmd, VkExtent2D extent, VkImage img, VkImageView view) {
        if (vk_backend && isenabled()) {
            vk_backend->render_menu(cmd, extent, img, view);
        }
    }
}
