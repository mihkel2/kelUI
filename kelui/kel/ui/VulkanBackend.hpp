#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include "drawing.hpp"

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

        VkBuffer vertexBuffer = VK_NULL_HANDLE;
        VkDeviceMemory vertexBufferMemory = VK_NULL_HANDLE;
        size_t currentVertexBufferSize = 0;


        VkImage fontImage = VK_NULL_HANDLE;
        VkDeviceMemory fontImageMemory = VK_NULL_HANDLE;
        VkImageView fontImageView = VK_NULL_HANDLE;
        VkSampler fontSampler = VK_NULL_HANDLE;
        VkDescriptorSetLayout descriptorSetLayout = VK_NULL_HANDLE;
        VkDescriptorPool descriptorPool = VK_NULL_HANDLE;
        VkDescriptorSet descriptorSet = VK_NULL_HANDLE;

        void createrenderpass();
        void creategraphicspipeline();
        void createvertexbuffer(size_t size);
        void updatevertexbuffer(const std::vector<Vertex>& vertices);

        void createfonttexture();
        void createdescriptors();

        uint32_t findmemorytype(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkShaderModule createshadermodule(const uint32_t* code, size_t size);
    };
}
