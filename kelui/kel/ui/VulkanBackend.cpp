#include "VulkanBackend.hpp"
#include <android/log.h>

// read vulkan.org before u change anything here
// after here is purley taken from that site and tweaked 2 work



namespace kel {

void VulkanBackend::init(const Context& context) {
    ctx = context;
    create_render_pass();
    create_graphics_pipeline();

}

void VulkanBackend::createrenderpass() {
    VkAttachmentDescription colorAttachment{};
    colorAttachment.format = ctx.format;
    colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
    colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_LOAD;
    colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    colorAttachment.initialLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkAttachmentReference colorAttachmentRef{};
    colorAttachmentRef.attachment = 0;
    colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

    VkSubpassDescription subpass{};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &colorAttachmentRef;

    VkRenderPassCreateInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = 1;
    renderPassInfo.pAttachments = &colorAttachment;
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpass;

    vkCreateRenderPass(ctx.device, &renderPassInfo, nullptr, &renderPass);
}

void VulkanBackend::creategraphicspipeline() {


}

void VulkanBackend::rendermenu(VkCommandBuffer cmd, VkExtent2D extent, VkImage targetImage, VkImageView targetView) {

    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = extent;


    VkFramebufferCreateInfo framebufferInfo{};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = renderPass;
    framebufferInfo.attachmentCount = 1;
    framebufferInfo.pAttachments = &targetView;
    framebufferInfo.width = extent.width;
    framebufferInfo.height = extent.height;
    framebufferInfo.layers = 1;

    VkFramebuffer framebuffer;
    vkCreateFramebuffer(ctx.device, &framebufferInfo, nullptr, &framebuffer);

    renderPassInfo.framebuffer = framebuffer;

    vkCmdBeginRenderPass(cmd, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


    vkCmdEndRenderPass(cmd);

    vkDestroyFramebuffer(ctx.device, framebuffer, nullptr);
}

void VulkanBackend::cleanup() {
    vkDestroyRenderPass(ctx.device, renderPass, nullptr);

}

}
