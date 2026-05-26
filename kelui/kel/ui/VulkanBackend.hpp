#pragma once
#include <vulkan/vulkan.h>
#include <vector>
//once again dont touch anything here skidded from imgui this time
namespace kel {
    class VulkanBackend {
    public:
        struct Context {
            VkInstance instance;
            VkPhysicalDevice physicalDevice;
            VkDevice device;
            VkQueue queue;
            uint32_t queueFamilyIndex;
            VkFormat format;
        };

        void init(const Context& ctx);
        void rendermenu(VkCommandBuffer cmd, VkExtent2D extent, VkImage targetImage, VkImageView targetView);
        void cleanup();

    private:
        Context ctx;
        VkRenderPass renderPass;
        VkPipelineLayout pipelineLayout;
        VkPipeline graphicsPipeline;

        void createrenderpass();
        void creategraphicspipeline();
    };
}
