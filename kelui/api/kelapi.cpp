#include "kel.hpp"
#include "../kel/ui/VulkanBackend.hpp"
#include "../kel/ui/drawing.hpp"
#include <iostream>
#include <vector>

namespace kel {
    static KelInstance* instance = nullptr;
    static VulkanBackend* vk_backend = nullptr;
    static KelPose last_controller_pose = {0,0,0, 0,0,0,1}; //OPENXR CONFIG FISHY FIX THIS

    KelInstance* getapi() {
        if (!instance) {
            instance = new KelInstance();
            vk_backend = new VulkanBackend();
        }
        return instance;
    }

    void KelInstance::start() {

    }

    void KelInstance::createtab(const std::string& name, float offX, float offY, float offZ) {
        tabs.push_back({name, {offX, offY, offZ, 0, 0, 0, 1}});
    }

    //void KelInstance::switchtab(int index) {
       // if (index >= 0 && index < tabs.size()) {
           // active_tab = index;
       // }
   // }

    void KelInstance::button(const std::string& name, bool* state, void(*callback)()) {
        // Here you would draw a button rect and handle touch logic
        //fishy add this

        if (state) *state = !(*state);
        if (callback) callback();
    }

    void KelInstance::rect(float x, float y, float w, float h, float r, float g, float b, float a) {
        Drawing::rect(x, y, w, h, {r, g, b, a});
    }

    void KelInstance::updatecontrollerpose(KelPose pose) {
        last_controller_pose = pose;
    }

    void KelInstance::rendervulkan(VkCommandBuffer cmd, VkExtent2D extent, VkImage img, VkImageView view) {
        if (!vk_backend || !isenabled()) return;

        // The drawing for the current frame should happen before or during this call
        // In a tabbed system, you'd only call the logic for the 'active_tab'

        vk_backend->rendermenu(cmd, extent, img, view);
        Drawing::clear();
    }
}
